/*
 * Copyright (c) 1997, 1998, 1999 Kungliga Tekniska H�gskolan
 * (Royal Institute of Technology, Stockholm, Sweden). 
 * All rights reserved. 
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met: 
 *
 * 1. Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 *
 * 3. All advertising materials mentioning features or use of this software 
 *    must display the following acknowledgement: 
 *      This product includes software developed by Kungliga Tekniska 
 *      H�gskolan and its contributors. 
 *
 * 4. Neither the name of the Institute nor the names of its contributors 
 *    may be used to endorse or promote products derived from this software 
 *    without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE 
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
 * SUCH DAMAGE. 
 */

#include "kadm5_locl.h"

RCSID("$Id$");

/*
 * free all the memory used by (len, keys)
 */

static void
free_keys (kadm5_server_context *context,
	   int len, Key *keys)
{
    int i;

    for (i = 0; i < len; ++i) {
	free (keys[i].mkvno);
	keys[i].mkvno = NULL;
	if (keys[i].salt != NULL) {
	    free_Salt(keys[i].salt);
	    free(keys[i].salt);
	    keys[i].salt = NULL;
	}
	krb5_free_keyblock_contents(context->context, &keys[i].key);
    }
}

/*
 * Set the keys of `ent' to the string-to-key of `password'
 */

kadm5_ret_t
_kadm5_set_keys(kadm5_server_context *context,
		hdb_entry *ent, 
		const char *password)
{
    kadm5_ret_t ret = 0;
    int i;
    unsigned len;
    Key *keys;
    krb5_salt salt;
    krb5_enctype des_types[] = { ETYPE_DES_CBC_CRC,
				 ETYPE_DES_CBC_MD4,
				 ETYPE_DES_CBC_MD5 };
    krb5_boolean v4_salt = FALSE;

    len = 4;
    keys = malloc (len * sizeof(*keys));
    if (keys == NULL)
	return ENOMEM;

    for (i = 0; i < len; ++i) {
	keys[i].mkvno               = NULL;
	keys[i].salt                = NULL;
	keys[i].key.keyvalue.length = 0;
	keys[i].key.keyvalue.data   = NULL;
    }

    salt.salttype         = KRB5_PW_SALT;
    salt.saltvalue.length = 0;
    salt.saltvalue.data   = NULL;

    if (!krb5_config_get_bool (context->context,
			       NULL, "kadmin", "use_v4_salt", NULL)) {
	ret = krb5_get_pw_salt (context->context, ent->principal, &salt);
	if (ret)
	    goto out;
    } else {
	v4_salt = TRUE;
    }

    for (i = 0; i < sizeof(des_types) / sizeof(des_types[0]); ++i) {
	ret = krb5_string_to_key_salt (context->context,
				       des_types[i],
				       password,
				       salt,
				       &keys[i].key);
	if (ret)
	    goto out;
	if (v4_salt) {
	    keys[i].salt = malloc (sizeof(*keys[i].salt));
	    if (keys[i].salt == NULL) {
		ret = ENOMEM;
		goto out;
	    }
	    keys[i].salt->type = salt.salttype;
	    ret = copy_octet_string (&salt.saltvalue, &keys[i].salt->salt);
	    if (ret)
		goto out;
	}
    }

    ret = krb5_string_to_key (context->context,
			      ETYPE_DES3_CBC_SHA1,
			      password,
			      ent->principal,
			      &keys[3].key);
    if (ret)
	goto out;

    free_keys (context, ent->keys.len, ent->keys.val);
    ent->keys.len = len;
    ent->keys.val = keys;
    ent->kvno++;
    return ret;
out:
    krb5_data_free (&salt.saltvalue);
    free_keys (context, len, keys);
    return ret;
}

/*
 * Set the keys of `ent' to (`n_key_data', `key_data')
 */

kadm5_ret_t
_kadm5_set_keys2(hdb_entry *ent, 
		 int16_t n_key_data, 
		 krb5_key_data *key_data)
{
    krb5_error_code ret;
    int i;

    ent->keys.len = n_key_data;
    ent->keys.val = malloc(ent->keys.len * sizeof(*ent->keys.val));
    if(ent->keys.val == NULL)
	return ENOMEM;
    for(i = 0; i < n_key_data; i++) {
	ent->keys.val[i].mkvno = NULL;
	ent->keys.val[i].key.keytype = key_data[i].key_data_type[0];
	ret = krb5_data_copy(&ent->keys.val[i].key.keyvalue,
			     key_data[i].key_data_contents[0],
			     key_data[i].key_data_length[0]);
	if(ret)
	    return ret;
	if(key_data[i].key_data_ver == 2) {
	    Salt *salt;
	    salt = malloc(sizeof(*salt));
	    if(salt == NULL)
		return ENOMEM;
	    ent->keys.val[i].salt = salt;
	    salt->type = key_data[i].key_data_type[1];
	    krb5_data_copy(&salt->salt, 
			   key_data[i].key_data_contents[1],
			   key_data[i].key_data_length[1]);
	} else
	    ent->keys.val[i].salt = NULL;
    }
    ent->kvno++;
    return 0;
}
