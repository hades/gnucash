/********************************************************************\
 * Group.h -- chart of accounts (hierarchical tree of accounts)     *
 * Copyright (C) 1997 Robin D. Clark                                *
 * Copyright (C) 1997-2000,2003 Linas Vepstas <linas@linas.org>     *
 *                                                                  *
 * This program is free software; you can redistribute it and/or    *
 * modify it under the terms of the GNU General Public License as   *
 * published by the Free Software Foundation; either version 2 of   *
 * the License, or (at your option) any later version.              *
 *                                                                  *
 * This program is distributed in the hope that it will be useful,  *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the    *
 * GNU General Public License for more details.                     *
 *                                                                  *
 * You should have received a copy of the GNU General Public License*
 * along with this program; if not, contact:                        *
 *                                                                  *
 * Free Software Foundation           Voice:  +1-617-542-5942       *
 * 59 Temple Place - Suite 330        Fax:    +1-617-542-2652       *
 * Boston, MA  02111-1307,  USA       gnu@gnu.org                   *
 *                                                                  *
\********************************************************************/

#ifndef XACC_ACCOUNT_GROUP_H
#define XACC_ACCOUNT_GROUP_H

#include <glib.h>

#include "Account.h"
#include "gnc-engine.h"
#include "guid.h"
#include "qofbook.h"


/** PROTOTYPES ******************************************************/
/*
 * The xaccMallocAccountGroup() routine will create a new account group.
 *    This is an internal-use function, you almost certainly want to
 *    be using the xaccGetAccountGroup() routine instead.
 */
AccountGroup *xaccMallocAccountGroup (QofBook *book);

/*
 * The xaccGetAccountGroup() routine will return the top-most
 * account group associated with the indicated book.
 */
AccountGroup * xaccGetAccountGroup (QofBook *book);
AccountGroup * xaccCollGetAccountGroup (QofCollection *col);

/*
 * The xaccAccountDestroy() routine will destroy and free all 
 *    the data associated with this account group.  The group
 *    must have been opened for editing with 
 *    xaccAccountGroupBeginEdit() first, before the Destroy is called.
 */
void          xaccAccountGroupDestroy (AccountGroup *grp);


/* XXX backwards-compat define, remove at later convenience */
#define gnc_book_get_group xaccGetAccountGroup

QofBook * xaccGroupGetBook (AccountGroup *group);

void          xaccAccountGroupBeginEdit (AccountGroup *grp);
void          xaccAccountGroupCommitEdit (AccountGroup *grp);

/*
 * The xaccGroupConcatGroup() subroutine will move (reparent) 
 *    all accounts from the "src" group to the "dest" group,
 *    preserving the account heirarchy.  It will also take care 
 *    that the moved accounts will have the "dest" group's book
 *    parent as well.
 *
 * The xaccGroupCopyGroup() subroutine will copy all accounts
 *    from the "src" group to the "dest" group, preserving the 
 *    account heirarchy.  It will also take care that the moved 
 *    accounts will have the "dest" group's book parent as well.
 *    This routine will *NOT* copy any splits/transactions.
 *    It will copy the KVP trees in each account.
 *
 * The xaccGroupMergeAccounts() subroutine will go through a group,
 *    merging all accounts that have the same name and description.
 *    This function is useful when importing Quicken(TM) files.
 */

void    xaccGroupConcatGroup (AccountGroup *dest, AccountGroup *src);
void    xaccGroupCopyGroup (AccountGroup *dest, AccountGroup *src);
void    xaccGroupMergeAccounts (AccountGroup *grp);

/*
 * The xaccGroupNotSaved() subroutine will return TRUE
 *    if any account in the group or in any subgroup
 *    hasn't been saved.
 *
 * The xaccGroupMarkSaved() subroutine will mark
 *    the entire group as having been saved, including 
 *    all of the child accounts.
 *
 * The xaccGroupMarkNotSaved() subroutine will mark
 *    the given group as not having been saved.
 */

gboolean xaccGroupNotSaved  (AccountGroup *grp);
void     xaccGroupMarkSaved (AccountGroup *grp);
void     xaccGroupMarkNotSaved (AccountGroup *grp);

/*
 * The xaccGroupInsertAccount() subroutine will insert the indicated
 *    account into the indicated group.  If it already is the child 
 *    of another group, it will be removed there first.  If the
 *    account belongs to a different book than the the group, it
 *    will be removed from the other book (and thus, the other book's
 *    entity tables, generating destroy & create events).  If the 
 *    account is removed from and inserted into the same group, the 
 *    overall account sort order will be recomputed.  
 *
 * The xaccAccountInsertSubAccount() does the same, except that
 *    the parent is specified as an account.
 */

void    xaccGroupInsertAccount (AccountGroup *grp, Account *acc);
void    xaccAccountInsertSubAccount (Account *parent, Account *child);

/*
 * The xaccGroupGetNumSubAccounts() subroutine returns the number
 *    of accounts, including subaccounts, in the account group
 *
 * The xaccGroupGetNumAccounts() subroutine returns the number
 *    of accounts in the indicated group only (children not counted).
 *
 * The xaccGroupGetDepth() subroutine returns the length of the 
 *    longest tree branch.  Each link between an account and its
 *    (non-null) children counts as one unit of length.
 */

int     xaccGroupGetNumSubAccounts (AccountGroup *grp);
int     xaccGroupGetNumAccounts (AccountGroup *grp);
int     xaccGroupGetDepth (AccountGroup *grp);
Account * xaccGroupGetAccount (AccountGroup *group, int index);

/*
 * The xaccGroupGetSubAccounts() subroutine returns an list of the accounts,
 *    including subaccounts, in the account group. The returned list
 *    should be freed with g_list_free() when no longer needed.
 *
 * The xaccGroupGetAccountList() subroutines returns only the immediate
 *    children of the account group. The returned list should *not*
 *    be freed by the caller.
 */

AccountList * xaccGroupGetSubAccounts (AccountGroup *grp);
AccountList * xaccGroupGetAccountList (AccountGroup *grp);

/* 
 * The xaccGetAccountFromName() subroutine fetches the
 *    account by name from the collection of accounts
 *    in the indicated AccountGroup group.  It returns NULL if the
 *    account was not found.
 *
 * The xaccGetAccountFromFullName() subroutine works like
 *    xaccGetAccountFromName, but uses fully-qualified names
 *    using the given separator.
 *
 * The xaccGetPeerAccountFromName() subroutine fetches the
 *    account by name from the collection of accounts
 *    in the same AccountGroup anchor group. It returns NULL if the
 *    account was not found.
 *
 * The xaccGetPeerAccountFromFullName() subroutine works like
 *     xaccGetPeerAccountFromName, but uses fully-qualified
 *     names using the given separator.
 */

Account *xaccGetAccountFromName     (AccountGroup *group, const char *name);
Account *xaccGetAccountFromFullName (AccountGroup *group,
                                     const char *name,
                                     const char separator);
Account *xaccGetPeerAccountFromName (Account *account, const char *name);
Account *xaccGetPeerAccountFromFullName (Account *acc,
                                         const char * name,
                                         const char separator);

/*
 * The xaccGroupGetRoot() subroutine will find the topmost 
 *    (root) group to which this group belongs.
 *
 * The xaccGetAccountRoot() subroutine will find the topmost 
 *    (root) group to which this account belongs.
 */
AccountGroup * xaccGroupGetRoot (AccountGroup *grp);
AccountGroup * xaccAccountGetRoot (Account *account);

/* The xaccGroupGetParentAccount() subroutine returns the parent
 * account of the group, or NULL.
 */
Account * xaccGroupGetParentAccount (AccountGroup *group);

/* The xaccGroupMapAccounts() routine will traverse the account 
      group, returning a list of accounts.  If the callback
      returns null for a given item, it won't show up in
      the result list.  You should free the returned list when
      you are done with it.
*/
typedef  gpointer (*AccountCallback)(Account *a, gpointer data);
AccountList *xaccGroupMapAccounts(AccountGroup *grp,
                             AccountCallback,
                             gpointer data);

/* The xaccGroupForEachAccount() method will traverse the AccountGroup
 *    tree, calling 'func' on each account.   Traversal will stop when
 *    func returns a non-null value, and the routine wil return with that 
 *    value.  If 'deeply' is FALSE, then only the immediate children of 
 *    the account will be traversed.  If TRUE, then the whole tree will
 *    be traversed.
 */

gpointer xaccGroupForEachAccount (AccountGroup *grp,
                                  AccountCallback,
                                  gpointer data,
                                  gboolean deeply);

gboolean xaccGroupEqual(AccountGroup *a, AccountGroup *b,
                        gboolean check_guids);

/*
 * The following functions provide support for "staged traversals"
 * over all of the transactions in an account or group.  The idea
 * is to be able to perform a sequence of traversals ("stages"),
 * and perform an operation on each transaction exactly once 
 * for that stage.  
 *
 * Only transactions whose current "stage" is less than the
 * stage of the current traversal will be affected, and they will
 * be "brought up" to the current stage when they are processed.
 *
 * For example, you could perform a stage 1 traversal of all the
 * transactions in an account, and then perform a stage 1 traversal of
 * the transactions in a second account.  Presuming the traversal of
 * the first account didn't abort prematurely, any transactions shared
 * by both accounts would be ignored during the traversal of the
 * second account since they had been processed while traversing the
 * first account.
 *
 * However, if you had traversed the second account using a stage 
 * of 2, then all the transactions in the second account would have 
 * been processed.
 *
 * Traversal can be aborted by having the callback function return
 * a non-zero value.  The traversal is aborted immediately, and the 
 * non-zero value is returned.  Note that an aborted traversal can 
 * be restarted; no information is lost due to an abort.
 *
 * The initial impetus for this particular approach came from
 * generalizing a mark/sweep practice that was already being
 * used in FileIO.c.
 *
 * Note that currently, there is a hard limit of 256 stages, which
 * can be changed by enlarging "marker" in the transaction struct.
 * */

/* xaccGroupBeginStagedTransactionTraversals() resets the traversal
 *    marker inside each of all the transactions in the group so that
 *    a new sequence of staged traversals can begin.
 *
 * xaccSplitsBeginStagedTransactionTraversals() resets the traversal
 *    marker for each transaction which is a parent of one of the
 *    splits in the list.
 *
 * xaccAccountBeginStagedTransactionTraversals() resets the traversal
 *    marker for each transaction which is a parent of one of the
 *    splits in the account.
 *
 */

void xaccGroupBeginStagedTransactionTraversals(AccountGroup *grp);
void xaccSplitsBeginStagedTransactionTraversals(SplitList *splits);
void xaccAccountBeginStagedTransactionTraversals(Account *account);

/* xaccTransactionTraverse() checks the stage of the given transaction.
 *    If the transaction hasn't reached the given stage, the transaction
 *    is updated to that stage and the function returns TRUE. Otherwise
 *    no change is made and the function returns FALSE.
 *
 * xaccSplitTransactionTraverse() behaves as above using the parent of
 *    the given split.
 */

gboolean xaccTransactionTraverse(Transaction *trans, int stage);
gboolean xaccSplitTransactionTraverse(Split *split, int stage);

/* xaccGroupStagedTransactionTraversal() calls thunk on each
 *    transaction in the group whose current marker is less than the
 *    given `stage' and updates each transaction's marker to be `stage'.
 *    The traversal will stop if thunk() returns a non-zero value.
 *    xaccGroupStagedTransactionTraversal() function will return zero 
 *    or the non-zero value returned by thunk().  This
 *    API does not handle handle recursive traversals.
 *
 *    Currently the result of adding or removing transactions during
 *    a traversal is undefined, so don't do that.
 */

int xaccGroupStagedTransactionTraversal(AccountGroup *grp,
                                    unsigned int stage,
                                    TransactionCallback,
                                    void *data);

/* xaccAccountStagedTransactionTraversal() calls thunk on each
 *    transaction in the account whose current marker is less than the
 *    given `stage' and updates each transaction's marker to be `stage'.
 *    The traversal will stop if thunk() returns a non-zero value.
 *    xaccAccountStagedTransactionTraversal() function will return zero
 *    or the non-zero value returned by thunk().
 *    This API does not handle handle recursive traversals.
 *
 *    Currently the result of adding or removing transactions during
 *    a traversal is undefined, so don't do that. 
 */

int xaccAccountStagedTransactionTraversal(Account *a,
                                          unsigned int stage,
                                          TransactionCallback,
                                          void *data);

/** Traverse all of the transactions in the given account group.
   Continue processing IFF proc returns 0. This function
   will descend recursively to traverse transactions in the
   children of the accounts in the group.

   Proc will be called exactly once for each transaction that is
   pointed to by at least one split in any account in the hierarchy
   topped by AccountGroup g.

   The result of this function will be 0 IFF every relevant
   transaction was traversed exactly once; otherwise, the return
   value is the last non-zero value returned by the callback.

   Note that the traversal occurs only over the transactions that
   are locally cached in the local gnucash engine.  If the gnucash
   engine is attached to a remote database, the database may contain
   (many) transactions that are not mirrored in the local cache.
   This routine will not cause an SQL database query to be performed;
   it will not traverse transactions present only in the remote
   database.

   Note that this routine is just a trivial wrapper for 
   
   xaccGroupBeginStagedTransactionTraversals(grp);
   xaccGroupStagedTransactionTraversal(grp, 42, cb, data);
 */

int xaccGroupForEachTransaction(AccountGroup *g, 
                                TransactionCallback, void *data);

#endif /* XACC_ACCOUNT_GROUP_H */
