/*
 * Copyright (c) 1997-1999 Kungliga Tekniska H�gskolan
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

/* 
 * $Id$
 */

#ifndef __ADMIN_LOCL_H__
#define __ADMIN_LOCL_H__

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_NETINET_IN6_H
#include <netinet/in6.h>
#endif
#ifdef HAVE_NETINET6_IN6_H
#include <netinet6/in6.h>
#endif

#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif
#ifdef HAVE_SYS_UN_H
#include <sys/un.h>
#endif
#include <err.h>
#include <roken.h>
#include <krb5.h>
#include <hdb.h>
#include <hdb_err.h>
#include <kadm5/admin.h>
#include <kadm5/private.h>
#include <kadm5/kadm5_err.h>
#include <parse_time.h>
#include <getarg.h>


extern krb5_context context;
extern void * kadm_handle;

#define DECL(X) int X(int, char **)

DECL(add_new_key);
DECL(cpw_entry);
DECL(del_entry);
DECL(exit_kadmin);
DECL(ext_keytab);
DECL(get_entry);
DECL(get_privs);
DECL(help);
DECL(list_princs);
DECL(mod_entry);
DECL(rename_entry);
DECL(init);
DECL(dump);
DECL(load);
DECL(merge);

#define ALLOC(X) ((X) = malloc(sizeof(*(X))))

#define KADMIN_APPL_VERSION "KADM0.0" /* XXX get from kadm5_locl.h */

/* util.c */

void timeval2str(time_t t, char *str, size_t len, int include_time);
void deltat2str(unsigned t, char *str, size_t len);
int str2deltat(const char *str, unsigned *delta);
void attr2str(krb5_flags attributes, char *str, size_t len);
int str2attr(const char *str, krb5_flags *flags);
void get_response(const char*, const char*, char*, size_t);
int get_deltat(const char*, const char*, unsigned *);
int edit_entry(kadm5_principal_ent_t ent, int *mask);
int foreach_principal(const char *, int (*)(krb5_principal, void*), void *);

/* server.c */

krb5_error_code
kadmind_loop (krb5_context, krb5_auth_context, krb5_keytab, int);

/* version4.c */

void
handle_v4(krb5_context context, int len, int fd);

#endif /* __ADMIN_LOCL_H__ */
