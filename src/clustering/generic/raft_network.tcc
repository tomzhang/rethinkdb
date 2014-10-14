// Copyright 2010-2014 RethinkDB, all rights reserved.
#ifndef CLUSTERING_GENERIC_RAFT_NETWORK_TCC_
#define CLUSTERING_GENERIC_RAFT_NETWORK_TCC_

#include "clustering/generic/raft_network.hpp"

template<class state_t>
raft_networked_member_t<state_t>::raft_networked_member_t(
        const raft_member_id_t &this_member_id,
        mailbox_manager_t *_mailbox_manager,
        watchable_map_t<raft_member_id_t, raft_business_card_t<state_t> > *_peers,
        raft_storage_interface_t<state_t> *storage,
        const raft_persistent_state_t<state_t> &persistent_state) :
    mailbox_manager(_mailbox_manager),
    peers(_peers),
    member(this_member_id, storage, this, persistent_state),
    rpc_mailbox(mailbox_manager,
        std::bind(&raft_networked_member_t::on_rpc, this,
            ph::_1, ph::_2, auto_drainer_t::lock_t(&drainer)))
    { }

template<class state_t>
raft_business_card_t<state_t> raft_networked_member_t<state_t>::get_business_card() {
    raft_business_card_t<state_t> bc;
    bc.request_vote = request_vote_mailbox.get_address();
    bc.install_snapshot = install_snapshot_mailbox.get_address();
    bc.append_entries = append_entries_mailbox.get_address();
}

template<class state_t>
bool raft_networked_member_t<state_t>::send_rpc(
        const raft_member_id_t &dest,
        const raft_rpc_request_t<state_t> &request,
        signal_t *interruptor,
        raft_rpc_reply_t *reply_out) {
    /* Find the given member's mailbox address */
    boost::optional<raft_business_card_t<state_t> > bcard = peers->get_key(dest);
    if (!static_cast<bool>(bcard)) {
        /* The member is not connected */
        return false;
    }
    /* Send message and wait for a reply */
    disconnect_watcher_t watcher(mailbox_manager, bcard.rpc.get_peer());
    cond_t got_reply;
    mailbox_t<void(raft_rpc_reply_t)> reply_mailbox(
        mailbox_manager,
        [&](raft_rpc_reply_t &&reply) {
            *reply_out = reply;
            got_reply.pulse();
        });
    send(mailbox_manager, addr, request, reply_mailbox.get_address());
    wait_any_t waiter(&watcher, &got_reply);
    wait_interruptible(&waiter, interruptor);
    return got_reply.is_pulsed();
}

clone_ptr_t<watchable_t<std::set<raft_member_id_t> > >
        get_connected_members() {
    ...
}

template<class state_t>
void raft_networked_member_t<state_t>::on_rpc(
        const raft_rpc_request_t<state_t> &request,
        const mailbox_t<void(raft_rpc_reply_t)>::address_t &reply_addr,
        auto_drainer_t::lock_t keepalive) {
    raft_rpc_reply_t reply;
    member.on_rpc(request, keepalive.get_drain_signal(), &reply);
    send(mailbox_manager, reply_addr, reply);
}

#endif   /* CLUSTERING_GENERIC_RAFT_NETWORK_TCC_ */

