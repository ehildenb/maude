---
title: "CS 476: Project: The Bitswap Protocol"
author: "Nishant Rodrigues"
geometry: "margin=2cm"
references:
- id: IPFS
  title: "IPFS - Content Addressed, Versioned, P2P File System"
  author:
  - family: Benet
    given: Juan
  URL: https://github.com/ipfs/papers/blob/master/ipfs-cap2pfs/ipfs-p2p-file-system.pdf
- id: Bitswap
  title: Bitswap specification
  URL: https://github.com/ipfs/specs/tree/master/bitswap
- id: altruism
  title: The nature of human altruism
  author:
  - family: Fehr
    given: Ernst
  - family: Fischbacher
    given: Urs
  container-title: Nature
  volume: 425
  number: 6960
  pages: 785
  issued:
    date-parts:
    - - 2003
  publisher: Nature Publishing Group
---

# Motivation

The Bitswap protocol, a part of the InterPlanetary File System [@IPFS] suite of protocols, is a
peer-to-peer protocol for exchanging documents and media over various communication channels. Nodes
advertise SHA1 cryptographic hashes of blocks of this data. A peer that has the corresponding block
may choose to respond with that block [@Bitswap]. In an ideal world, peers on the network would
altruistically send the any block they have to a peer immediately. However, bandwidth is a scarce
resource, there are limits to human altruism[@altruism], and some actors on the network may be
malicious. We must thus be cautious about how we share the blocks we have available.

# Implementation

While we could choose to model the Bitswap protocol at a low level, defining the semantics of each
low level message, it is the analysis of the game theoretic aspects of the various strategies that
nodes implement that is most interesting. Thus, we assume that all actors have full and
instantaneous knowledge about which blocks their peers are interested in and that sharing this
information has zero bandwidth cost. It is also convenient to model time discretely. Additionally,
we force each node to take turns, as though sitting around a table playing a board game. This
ignores the possiblity of races and other real-world situations. Below, we define in Maude a system
module, `BITSWAP-TURN-BASED-GAME`, that does exactly this. Note that we elide some details, though
the full source may be found at
<https://github.com/ehildenb/maude/blob/systems/bitswap/contrib/systems.md/bitswap.md>.

``` {.maude}
mod BITSWAP-TURN-BASED-GAME is
```

``` {.maude pipe="cat > /dev/null"}
    protecting QID-SET .
    protecting QID-LIST .
    protecting INT .
```

We use quoted identifies to represent identifiers for peers on the network, as well as SHAs of
blocks and the blocks themselves.

``` {.maude}
    sort Node Strategy NodeId BlockSet WorldState .

    subsorts Qid < NodeId .
    subsorts QidSet < BlockSet .
    subsorts Node < WorldState .
```

Sometimes a node decides to *not* send a block to any peer on a round of the game. The constructor
`nobody` is used to represent that case.

``` {.maude}
    op nobody : -> NodeId                                               [ctor] .
```

We model each node as a four-tuple consisting of an identifier, the strategy it applies for
interacting with the network, and a set of blocks it has, and another that it wants.

``` {.maude}
    op [ _ _ _ _ ] : NodeId Strategy BlockSet BlockSet -> Node          [ctor] .
    eq [ A S1 (BLOCK, H1) (BLOCK, W1) ]
     = [ A S1 (BLOCK, H1) (W1)        ]
       .
```

Each strategy is defined by three operations:

``` {.maude}
    op send-next-block-to    : Strategy QidSet -> NodeId   .
    op record-block-sent     : Strategy NodeId -> Strategy .
    op record-block-received : Strategy NodeId -> Strategy .
```

The `send-next-block-to` allows the model to query the strategy as to which peer it would like to
send a block to, while `record-block-sent` and `record-block-received` allow the strategy to update
its internal book keeping.

We define the world state as simply a collection of `Node`s.

``` {.maude}
    op empty : -> WorldState                                            [ctor] .
    op _ _   : WorldState WorldState -> WorldState
                                    [ctor assoc comm id: empty format(d ni d)] .
```

``` {.maude pipe='cat > /dev/null'}
    vars N : Int .
    vars NS : WorldState .
    vars A B ID : NodeId .
    vars IDS : QidSet .
    vars BLOCK : Qid . vars H1 W1 H2 W2 : BlockSet .
    vars S1 S2 : Strategy .
    vars IDLIST IDLIST1 IDLIST2 : QidList .
```

The core mechanism for describing the game is next `tick` rule that defines the transition from
one `WorldState` to the next.

``` {.maude}
    op tick(_,_) : Nat WorldState -> WorldState [format(nss d d d ++++++++++ d n)] .
```

During each tick, from a list of candidate nodes (that have some block that ...) each in turn node
picks a single peer to share a block with, and both sender and recipient update their states to
record this transaction. This is implemented in the following block of code.

``` {.maude}
    eq tick(N, NS) = $tick.foreachNode(N, get-ids(NS), NS)
       [print "---- Begin -----------------" ]
       .

    op $tick.foreachNode : Nat QidSet WorldState -> WorldState .
    eq $tick.foreachNode(0   , IDS, NS) = NS .
    eq $tick.foreachNode(s(N), IDS, NS) = $tick.foreachNode(N, IDS, $tick.foreachNode(IDS, NS)) .

    op $tick.foreachNode                 :             QidSet WorldState -> WorldState .
    op $tick.foreachNode.chooseRecepient : Qid Qid     QidSet WorldState -> WorldState .
    op $tick.foreachNode.chooseBlock     : Qid Qid Qid QidSet WorldState -> WorldState .
    rl $tick.foreachNode(empty, NS) => NS
       [ print "---- Tick -----------------" ]
       .
    rl $tick.foreachNode((A, IDS), [A S1 H1 W1] NS)
    => $tick.foreachNode.chooseRecepient(A,
             send-next-block-to(S1, could-send-to(H1, NS)), IDS, [A S1 H1 W1] NS) .
    rl $tick.foreachNode.chooseRecepient(A, B,
             IDS, [ A S1 H1 W1 ] [ B S2 H2 W2 ] NS)
    => $tick.foreachNode.chooseBlock    (A, B, take(intersection(H1, W2)),
             IDS, [ A S1 H1 W1 ] [ B S2 H2 W2 ] NS) .

    rl $tick.foreachNode.chooseRecepient(A, nobody, IDS, [ A S1 H1 W1 ] NS)
    => $tick.foreachNode(IDS, [ A record-block-sent    (S1, nobody) H1 W1 ] NS)
       .
    rl $tick.foreachNode.chooseBlock(A, B, BLOCK, IDS, [ A S1 H1 W1 ] [ B S2 H2 W2 ] NS)
    => $tick.foreachNode(IDS, [ A record-block-sent    (S1, B)  H1         W1 ]
                              [ B record-block-received(S2, A) (H2, BLOCK) W2 ]
                              NS)
      [print "    " A " sent block " BLOCK " to " B]
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
strategy. This strategy chooses an arbitrary node to respond to:

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

``` {.test pipe='cat > /dev/null'}
load ../../contrib/systems/bitswap.maude
```

``` {.test pipe='maude -no-banner -no-wrap root/systems/bitswap.maude | grep -v "========\\|rewrites: \\|Bye" ' }
set print attribute on .
rewrite tick(3, [ 'a naive ('x, 'y, 'z) ('p, 'q, 'r) ]
                [ 'b naive ('p, 'q, 'r) ('x, 'y, 'z) ]
            ) .
```

Further, when a "needy" node joins the network, with no blocks that others desire
but in-need of blocks, it is able to get its feet off the ground.

``` {.test pipe='maude -no-banner -no-wrap root/systems/bitswap.maude | grep -v "========\\|rewrites: \\|Bye" ' }
rewrite tick(3, [ 'a     naive ('x, 'y, 'z) ('p, 'q, 'r) ]
                [ 'needy naive empty        ('x, 'y, 'z) ]
            ) .
```

However, things don't work out as well in the presence of a "selfish" node --
one that refuses to share data with others and acts as a parasite taking advantage
of the generosity of its peers.

``` {.maude}
    op selfish : -> Strategy                                            [ctor] .
    eq send-next-block-to(selfish,    IDS ) = nobody                           .
    eq record-block-sent    (selfish, A)    = selfish                          .
    eq record-block-received(selfish, A)    = selfish                          .
```

For example, other nodes may get starved in its presence.

``` {.test pipe='maude -no-banner -no-wrap root/systems/bitswap.maude | grep -v "========\\|rewrites: \\|Bye" ' }
search tick(3, [ 'a naive   ('x, 'y, 'z) ('p, 'q, 'r) ]
               [ 'b selfish ('p, 'q, 'r) ('x, 'y, 'z) ]
               [ 'c naive   ('p, 'q, 'r) ('x, 'y, 'z) ]
           )
   =>* [ 'c naive ('p, 'q, 'r) ('x, 'y, 'z) ]
       NS
    .
```

The `round-robin` strategy offers a defense. Here, we prioritize for the peers we have least
recently sent a block to. For that we must mainain a list of `NodeId`s in order of how
recently we sent them a block.

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

Now, other nodes on the network aren't as susceptible to chance in the presence of a selfish node.

``` {.test pipe='maude -no-banner -no-wrap root/systems/bitswap.maude | grep -v "========\\|rewrites: \\|Bye" ' }
search tick(3, [ 'a round-robin('b 'c) ('x, 'y, 'z) ('p, 'q, 'r) ]
               [ 'b selfish            ('p, 'q, 'r) ('x, 'y, 'z) ]
               [ 'c round-robin('a 'b) ('p, 'q, 'r) ('x, 'y, 'z) ]
           )
   =>! [ 'b naive ('p, 'q, 'r) ('x, 'y, 'z) ]
       NS
    .
```

However, it seems wrong that free-loaders can do as well on the network as other well meaning nodes
actively trying to improve the state of the network. There must be a way to incentivize other peers
to share. Thus we implement the "tit-for-tat" strategy, where a node keeps track of how often another
node helps it out and tries to reciprocate.

``` {.maude}
    sort BalanceSheet .
    op empty :           -> BalanceSheet                                [ctor] .
    op _ |-> _ : Qid Int -> BalanceSheet                                [ctor] .
    op _,_ : BalanceSheet BalanceSheet -> BalanceSheet  [ctor assoc id: empty] .
```

Notice that `BalanceSheet` is not commutative, since we still need to maintain a round-robin
like structure to prevent starvation of other nodes.

``` {.maude}
    vars BS BS1 BS2 : BalanceSheet .

    op tit-for-tat : BalanceSheet -> Strategy                           [ctor] .
    eq send-next-block-to(tit-for-tat(((A |-> N), BS)), (IDS))
     = if   N >= 0 and A in IDS
       then A
       else send-next-block-to(tit-for-tat((BS)), (IDS))
       fi .
    eq send-next-block-to(tit-for-tat(empty), IDS) = nobody .
    eq record-block-sent(tit-for-tat((BS1, (A |-> N), BS2)), A)
     =                   tit-for-tat((BS1, BS2, (A |-> (N - 1))))              .
    eq record-block-sent(tit-for-tat(BS), nobody) = tit-for-tat(BS)            .
    eq record-block-received(tit-for-tat((BS1, (A |->  N     ), BS2)), A)
     =                       tit-for-tat((BS1, (A |-> (N + 1)), BS2))          .
    eq record-block-received(tit-for-tat(BS), nobody) = tit-for-tat(BS)        .
```

This seems to work great!

``` {.test pipe='maude -no-banner -no-wrap root/systems/bitswap.maude | grep -v "========\\|rewrites: \\|Bye" ' }
rewrite tick(5, [ 'a tit-for-tat((('b |-> 0), ('c |-> 0))) ('x, 'y, 'z) ('p, 'q, 'r) ]
                [ 'b selfish                               ('p, 'q, 'r) ('x, 'y, 'z) ]
                [ 'c tit-for-tat((('b |-> 0), ('a |-> 0))) ('p, 'q, 'r) ('x, 'y, 'z) ]
            )
        .
```

The selfish node is quickly left out of the proceedings.

``` {.test pipe='maude -no-banner -no-wrap root/systems/bitswap.maude | grep -v "========\\|rewrites: \\|Bye" ' }
search tick(5, [ 'a tit-for-tat((('b |-> 0), ('c |-> 0))) ('x, 'y, 'z) ('p, 'q, 'r) ]
               [ 'b selfish                               ('p, 'q, 'r) ('x, 'y, 'z) ]
               [ 'c tit-for-tat((('b |-> 0), ('a |-> 0))) ('p, 'q, 'r) ('x, 'y, 'z) ]
            )
    =>* [ 'b selfish ('p, 'q, 'r) ('x, 'y, 'z) ]
       NS
       .
```

But, wait. The "needy" node has been left in a tough situation. It has just got on to the network and
is not able to offer any blocks to its peers. It has been caught in the crossfire.

``` {.test pipe='maude -no-banner -no-wrap root/systems/bitswap.maude | grep -v "========\\|rewrites: \\|Bye" ' }
rewrite tick(3, [ 'a     tit-for-tat((('needy |-> 0))) ('x, 'y, 'z) ('p, 'q, 'r) ]
                [ 'needy naive                         empty        ('x, 'y, 'z) ]
            ) .
```

To remedy this situation, we decide to become a little more lenient. Every three `tick`s, we fallback
to the round-robin mode.

``` {.maude}
    op tft-generous : Nat BalanceSheet -> Strategy                       [ctor] .
    eq send-next-block-to(tft-generous(N, BS), IDS)
     = if 3 divides N
       then send-next-block-to(round-robin(balance-sheet-to-list(BS)), IDS)
       else send-next-block-to(tit-for-tat(BS), IDS)
       fi                                                                      .
    eq record-block-sent(tft-generous(N, BS), B)
     = tft-generous(N + 1, steal-balance-sheet(record-block-sent(tit-for-tat(BS), B)))     .
    eq record-block-received(tft-generous(N, BS), B)
     = tft-generous(N    , steal-balance-sheet(record-block-received(tit-for-tat(BS), B))) .

    op steal-balance-sheet : Strategy ~> BalanceSheet .
    eq steal-balance-sheet(tit-for-tat(BS)) = BS .
    op balance-sheet-to-list : BalanceSheet -> QidList .
    eq balance-sheet-to-list((A |-> N), BS) = A balance-sheet-to-list(BS)      .
    eq balance-sheet-to-list(empty) = nil                                      .
```

This seems to work much better!

``` {.test pipe='maude -no-banner -no-wrap root/systems/bitswap.maude | grep -v "========\\|rewrites: \\|Bye" ' }
set print attribute on .
rewrite tick(7, [ 'a     tft-generous(0, (('needy |-> 0))) ('x, 'y, 'z) ('p, 'q, 'r) ]
                [ 'needy naive                            empty        ('x, 'y, 'z) ]
            ) .
```

Finally, we demonstrate that this strategy works even when three players need to cooperate.

``` {.test pipe='maude -no-wrap -no-banner root/systems/bitswap.maude | grep -v "========\\|rewrites: \\|Bye" ' }
set print attribute on .
rewrite tick(4, [ 'a     tft-generous(0, (('b |-> 0), ('c |-> 0))) ('x, 'y) ('l, 'm) ]
                [ 'b     tft-generous(0, (('c |-> 0), ('a |-> 0))) ('l, 'm) ('p, 'q) ]
                [ 'c     tft-generous(0, (('a |-> 0), ('b |-> 0))) ('p, 'q) ('x, 'y) ]
            ) .
```

It is clear that the strategy is not optimal in this scenario. Perhaps we should be more generous
when we have a net debt to other players on the network? This will often be the case when the
network is bootstrapping and few nodes have built up a reputation amongst its peers.

# Conclusion

Although this is similar to the traditional Iterative Prisoners Dilemma problem, it is muddied by
the fact that there are multiple players and that these players can enter and leave the game at
will. In the real world, players can easily even change their identity mid-game, making it more
difficult to subsidize young nodes that have just joint the network. These, and a myriad of other
attacks, need to be taken into account. There are many variations of these strategies for doing
exactly that. Fortunately, the expressiveness and simplicity of Maude make exploring these many
possibilities easy.

``` {.maude pipe="cat > /dev/null"}
endm
```

``` {.test pipe="cat > /dev/null"}
reduce could-send-to(('p, 'q, 'r), ['a round-robin('b 'c) ('x, 'y, 'z) ('p, 'q, 'r)]) == 'a .
reduce could-send-to(('x, 'p, 'z), ['a round-robin('b 'c) ('x, 'y, 'z) ('p, 'q, 'r)]) == 'a .
reduce could-send-to(('x, 'y, 'z), ['a round-robin('b 'c) ('x, 'y, 'z) ('p, 'q, 'r)]) == empty .

reduce take(intersection(('a, 'b, 'c), ('b, 'c, 'd, 'e))) .
reduce take(intersection(('a, 'b, 'c), ('d, 'e))) == empty .

rewrite tick(2, empty) .

rewrite tick(2, [ 'a round-robin(('b 'c)) ('x, 'y, 'z) ('p, 'q, 'r) ]) .
rewrite tick(4, [ 'a round-robin(('b 'c)) ('x, 'y, 'z) ('p, 'q, 'r) ]
                [ 'b round-robin(('a 'c)) ('p, 'q, 'r) ('x, 'y, 'z) ]
            ) .
rewrite tick(2, [ 'a round-robin(('b 'c)) ('x, 'y, 'z) ('p, 'q, 'r) ]
                [ 'b round-robin(('a 'c)) ('x, 'y, 'z) ('p, 'q, 'r) ]
            ) .
```

# References

