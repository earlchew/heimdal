/*	$NetBSD: extern.h,v 1.2 1995/04/11 02:44:49 cgd Exp $	*/

/*-
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)extern.h	8.2 (Berkeley) 4/4/94
 */

#ifndef _EXTERN_H_
#define _EXTERN_H_

#include <netinet/in.h>
#include <netdb.h>

#include <stdio.h>
#include <stdarg.h>
#include <setjmp.h>
#include <pwd.h>

void	blkfree(char **);
char  **copyblk(char **);
void	cwd(char *);
void	delete(char *);
void	dologout(int);
void	fatal(char *);
int	ftpd_pclose(FILE *);
FILE   *ftpd_popen(char *, char *);
char   *getline(char *, int);
void	logwtmp(char *, char *, char *);
void	lreply(int, const char *, ...);
void	makedir(char *);
void	nack(char *);
void	pass(char *);
void	passive(void);
void	perror_reply(int, char *);
void	pwd(void);
void	removedir(char *);
void	renamecmd(char *, char *);
char   *renamefrom(char *);
void	reply(int, const char *, ...);
void	retrieve(char *, char *);
void	send_file_list(char *);
void	setproctitle(const char *, ...);
void	statcmd(void);
void	statfilecmd(char *);
void	store(char *, char *, int);
void	upper(char *);
void	user(char *);
void	yyerror(char *);

extern struct sockaddr_in ctrl_addr, his_addr;
extern char hostname[];

extern	struct sockaddr_in data_dest;
extern	int logged_in;
extern	struct passwd *pw;
extern	int guest;
extern	int logging;
extern	int type;
extern	int oobflag;
extern off_t file_size;
extern off_t byte_count;
extern jmp_buf urgcatch;

extern	int form;
extern	int debug;
extern	int timeout;
extern	int maxtimeout;
extern  int pdata;
extern	char hostname[], remotehost[];
extern	char proctitle[];
extern	int usedefault;
extern  int transflag;
extern  char tmpline[];


#endif /* _EXTERN_H_ */
