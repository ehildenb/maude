

The bitswap protocol is a peer-to-peer protocol for exchanging documents and media over various
communication channels. We simply call these "blocks" of data from here on. Nodes advertise the
md5sum of blocks they want, and their peers respond with any blocks they already have.

- ideal world we'd simply send any node the dta it needs.
- but bandwidth scarse.
- nodes malicious

- turn based game

Here, we model at a high level some stratergies available to us.

In this model, we assume that all nodes are aware of the wants of all other nodes
all the time.

```maude
fmod BITSWAP-TURN-BASED-GAME is
    protecting QID-SET .
    sort Node Strategy NodeId BlockSet WorldState .

    subsorts Qid < NodeId .
    subsorts BlockSet < QidSet .
    
    op nobody : -> NodeId                               [ctor] .
 
    op [ _ _ _ _ _ ] : NodeId Strategy BlockSet BlockSet -> Node [ctor] .

    op send-next-block-to    : Strategy QidSet       -> NodeId   .
    op record-block-sent     : Strategy NodeId       -> Strategy .
    op record-block-received : Strategy NodeId       -> Strategy .

    subsorts Node < WorldState .
    op empty : -> WorldState [ctor] .
    op _   _ : -> WorldState [ctor assoc comm id: empty] .

    op tick : WorldState -> WorldState .
    eq tick(empty) = empty .
    eq tick(NS) = $tick.foreachNode(NS, NS) .
   ceq $tick.foreachNode(A IDS,
                        [ A S1 H1 W1 ]
                        [ B S2 H2 W2 ]
                        NODES
                        )
     = $tick.foreachNode(IDS, 
                         [ A record-block-sent(S1)     H1 W1 ]
                         [ B record-block-received(S2) H2 W2 ]
                         NODES
                        )
   if send-next-block-to(S1, could-send-to([ B S2 H2 W2 ] NODES)) == B
    .
endfm
```
