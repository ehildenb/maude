The bitswap protocol is a peer-to-peer protocol for exchanging documents and media over various
communication channels. Nodes advertise SHA1 cryptographic hashes of blocks of this data. A peer
that has the corresponding block may chose to respond with that block. In an ideal world, peers on
the network would altruistically send the any block they have to a peer immediatly. However,
bandwidth is a scarse resource, and some actors on the network may be malicious. We must thus be
cautious about how freely we share the blocks we have available.

While we could chose to model the bitswap protocol at a low level, defining the semantics
of each low level message, the more interesting aspect for analysis the the game theoretic
aspects of the various strategies that nodes can choose. Thus, we assume that
all actors have full and instantanous knowledge about which blocks other nodes are interested
in and that sharing this information has zero bandwidth cost.

```maude
mod BITSWAP-TURN-BASED-GAME is
    protecting QID-SET .
    protecting QID-LIST .
    sort Node Strategy NodeId BlockSet WorldState .

    subsorts Qid < NodeId .
    subsorts QidSet < BlockSet .
    subsorts Node < WorldState .
    op nobody : -> NodeId                                               [ctor] .
```

We model each node as an identifier, the strategy it applies for interacting with the network,
and a set of blocks it has, and another that it wants.

``` {.maude}
    op [ _ _ _ _ ] : NodeId Strategy BlockSet BlockSet -> Node          [ctor] .
    eq [ A S1 (BLOCK, H1) (BLOCK, W1) ]
     = [ A S1 (BLOCK, H1) (W1)        ]
     .
```

Each strategy is defined by three operations:

``` {.maude}
    op send-next-block-to    : Strategy QidSet       -> NodeId   .
    op record-block-sent     : Strategy NodeId       -> Strategy .
    op record-block-received : Strategy NodeId       -> Strategy .
```

The `send-next-block-to` allows the model to query the strategy as to which peer it would like to
send a block to, while `record-block-sent` and `record-block-received` allow the strategy to update
its internal book keeping.

We define the world state as simply a collection of `Node`s.

``` {.maude}
    op empty : -> WorldState                                            [ctor] .
    op _ _   : WorldState WorldState -> WorldState [ctor assoc comm id: empty] .
```

```maude
    vars NS : WorldState .
    vars A B : Qid . vars IDS : QidSet .
    vars BLOCK : Qid . vars H1 W1 H2 W2 : BlockSet .
    vars S1 S2 : Strategy .
    vars IDLIST IDLIST1 IDLIST2 : QidList .
```

The core mechanism for describing the game is next `tick` rule that defines the transition from
one `WorldState` to the next.

``` {.maude}
    op tick : WorldState -> WorldState .
```

During each tick, from a list of candidate nodes (that have some block that ...) each in turn node
picks a single peer to share a block with, and both sender and recipient update their states to
record this transaction. This is implemented in the following block of code.

``` {.maude}
    eq tick(NS) = $tick.foreachNode(get-ids(NS), NS) .

    op $tick.foreachNode        : QidSet WorldState -> WorldState .
    op $tick.foreachNode.update : Qid Qid QidSet WorldState -> WorldState .
    eq $tick.foreachNode(empty, NS) = NS .
   crl $tick.foreachNode((A, IDS), [A S1 H1 W1] NS)
    => $tick.foreachNode.update(A, B, IDS, [A S1 H1 W1] NS)
    if B := send-next-block-to(S1, could-send-to(H1, NS))
       [print A " -> " B]
     .
    rl $tick.foreachNode.update(A, B, IDS, [ A S1 H1 W1 ] [ B S2 H2 W2 ] NS)
    => $tick.foreachNode(IDS, [ A record-block-sent(S1, B) H1 W1 ]
                              [ B record-block-received(S2, A) (H2, take(intersection(H1, W2))) W2 ]
                              NS)
     .
    rl $tick.foreachNode.update(A, nobody, IDS, [ A S1 H1 W1 ] NS)
    => $tick.foreachNode(IDS, [ A record-block-sent(S1, nobody) H1 W1 ] NS)
     .

    op take : BlockSet -> BlockSet .
    rl take((A, IDS)) => A .
    rl take(empty)    => empty .

    op get-ids : WorldState -> QidSet .
    eq get-ids(empty) = empty .
    eq get-ids([ A S1 H1 W1 ] NS) = A, get-ids(NS) .

    op could-send-to : BlockSet WorldState -> QidSet .
    eq could-send-to(H1, empty) = empty .
    eq could-send-to((BLOCK, H1), [B S2 H2 (BLOCK, W1)] NS) = B, could-send-to((BLOCK, H1), NS) .
    eq could-send-to((BLOCK, H1), [B S2 H2         W1 ] NS) =    could-send-to((BLOCK, H1), NS) [owise] .
```

In our first iteration we start with the simplest possible strategy: the `naive`
strategy. This strategy choses an arbitary node to respond to:

``` {.maude}
    op naive : -> Strategy                                              [ctor] .
    eq send-next-block-to(naive, (A, IDS)) = A                                 .
    eq send-next-block-to(naive,     IDS ) = nobody                    [owise] .
    eq record-block-sent    (naive, A) = naive .
    eq record-block-received(naive, A) = naive .
```

This strategy seems to work: two nodes implementing this strategy
efficiently and expeditely share blocks, reaching a state where they no longer
need any more blocks.

```test
load ../../contrib/systems/bitswap.maude
set print attribute on .
```

```test
rewrite tick(3, [ 'a naive ('x, 'y, 'z) ('p, 'q, 'r) ]
                [ 'b naive ('p, 'q, 'r) ('x, 'y, 'z) ]
            ) .
```

Further, when a "needy" node joins the network, with no blocks that others desire
but in-need of blocks, it is able to get its feet of the ground.

```test
rewrite tick(3, [ 'a     naive ('x, 'y, 'z) ('p, 'q, 'r) ]
                [ 'needy naive empty        ('x, 'y, 'z) ]
            ) .
```

However, things don't work out as well in the presence of a "selfish" node --
one that refuses to share data with others and acts as a parasite taking advantage
of the generosity of its peers.

``` {.maude}
    op selfish : -> Strategy                                            [ctor] .
    eq send-next-block-to(selfish,     IDS ) = nobody                          .
    eq record-block-sent    (selfish, A)     = selfish .
    eq record-block-received(selfish, A)     = selfish .
```

For example, other nodes may get starved in its presence.

```test
--- TODO: Search space is very large
--- search [1] in BITSWAP-TURN-BASED-GAME :
rewrite
       tick(3, [ 'a naive   ('x, 'y, 'z) ('p, 'q, 'r) ]
               [ 'b selfish ('p, 'q, 'r) ('x, 'y, 'z) ]
               [ 'c naive   ('p, 'q, 'r) ('x, 'y, 'z) ]
           )
---   =>! [ 'b naive ('p, 'q, 'r) ('x, 'y, 'z) ]
---       NS
    .
```

The `round-robin` strategy offers a defence. We keep track of when we last
have sent a block to a node and prioritize peer we least recently sent on to.

``` {.maude}
    op round-robin : QidList -> Strategy                                [ctor] .
    eq send-next-block-to(round-robin((A IDLIST)), (A, IDS)) = A               .
    eq send-next-block-to(round-robin((A IDLIST)), IDS)
     = send-next-block-to(round-robin((IDLIST)), IDS)                  [owise] .
    eq send-next-block-to(round-robin(nil), IDS) = nobody .
    eq record-block-sent(round-robin((IDLIST1 A IDLIST2)), A)
     =                   round-robin((IDLIST1 IDLIST2 A))
     .
    eq record-block-sent(round-robin(IDLIST), nobody)     = round-robin(IDLIST) .
    eq record-block-received(round-robin(IDLIST), A     ) = round-robin(IDLIST) .
    eq record-block-received(round-robin(IDLIST), nobody) = round-robin(IDLIST) .
```

Now, other nodes on the network aren't as suceptible to chance in the presence of a selfish node.
However, it seems wrong that free-loaders can do as well on the network as other well meaning nodes
actively trying to improve the state of the network. There must be a way to incentivise other peers
to share. Thus we implement the "tit-for-tat" strategy where a node keeps track of
how often another node helps it out and tries to reciprocate.

``` {.maude}
    sort BalanceSheet .
    op tit-for-tat : BalanceSheet -> Strategy                           [ctor] .
    --- TODO
```

This seems to work great! The selfish node is quickly denied.

```test
```

But, wait. The needy node has been left in a tough situation. It has just got on to the network
hand is not able to offer any blocks to its peers. It has been caught in the crossfire.

```test
```

To remedy this situation, we decide to become a little more lenient. We allow nodes to have a credit
of 5 blocks, and even in this situation every now and again we send the node a block.

``` {.maude}
    op tit-for-tat-lenient : BalanceSheet -> Strategy                        [ctor] .
```

This seems to work much better!

```maude
endm
```

```test
reduce could-send-to(('p, 'q, 'r), ['a round-robin('b 'c) ('x, 'y, 'z) ('p, 'q, 'r)]) == 'a .
reduce could-send-to(('x, 'p, 'z), ['a round-robin('b 'c) ('x, 'y, 'z) ('p, 'q, 'r)]) == 'a .
reduce could-send-to(('x, 'y, 'z), ['a round-robin('b 'c) ('x, 'y, 'z) ('p, 'q, 'r)]) == empty .

reduce take(intersection(('a, 'b, 'c), ('b, 'c, 'd, 'e))) .
reduce take(intersection(('a, 'b, 'c), ('d, 'e))) == empty .

rewrite tick(empty) .

rewrite tick([ 'a round-robin(('b 'c)) ('x, 'y, 'z) ('p, 'q, 'r) ]) .
rewrite tick([ 'a round-robin(('b 'c)) ('x, 'y, 'z) ('p, 'q, 'r) ]
             [ 'b round-robin(('a 'c)) ('p, 'q, 'r) ('x, 'y, 'z) ]
            ) .
rewrite tick([ 'a round-robin(('b 'c)) ('x, 'y, 'z) ('p, 'q, 'r) ]
             [ 'b round-robin(('a 'c)) ('x, 'y, 'z) ('p, 'q, 'r) ]
            ) .
```
