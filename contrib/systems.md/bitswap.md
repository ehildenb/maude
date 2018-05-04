

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
    protecting QID-LIST . 
    sort Node Strategy NodeId BlockSet WorldState .

    subsorts Qid < NodeId .
    subsorts QidSet < BlockSet .
    subsorts Node < WorldState .

    op nobody : -> NodeId                                               [ctor] .
 
    op [ _ _ _ _ ] : NodeId Strategy BlockSet BlockSet -> Node          [ctor] .
    eq [ A S1 (BLOCK, H1) (BLOCK, W1) ]
     = [ A S1 (BLOCK, H1) (W1)        ]
     .

    op send-next-block-to    : Strategy QidSet       -> NodeId   .
    op record-block-sent     : Strategy NodeId       -> Strategy .
    op record-block-received : Strategy NodeId       -> Strategy .

    op empty : -> WorldState                                            [ctor] .
    op _ _   : WorldState WorldState -> WorldState [ctor assoc comm id: empty] .

    vars NS : WorldState .
    vars A B : Qid . vars IDS : QidSet . 
    vars BLOCK : Qid . vars H1 W1 H2 W2 : BlockSet .
    vars S1 S2 : Strategy . 
    vars IDLIST IDLIST1 IDLIST2 : QidList .
   
    --- TODO: This is non-deterministic. Should become lists?
    op take : BlockSet -> BlockSet .
    eq take((A, IDS)) = A .
    eq take(empty)    = empty .

    op get-ids : WorldState -> QidSet .
    eq get-ids(empty) = empty .
    eq get-ids([ A S1 H1 W1 ] NS) = A, get-ids(NS) .

    op could-send-to : BlockSet WorldState -> QidSet .
    eq could-send-to(H1, empty) = empty .
    eq could-send-to((BLOCK, H1), [B S2 H2 (BLOCK, W1)] NS) = B, could-send-to((BLOCK, H1), NS) .
    eq could-send-to((BLOCK, H1), [B S2 H2        W1 ] NS) =    could-send-to((BLOCK, H1), NS) [owise] .

    op tick : WorldState -> WorldState .
    eq tick(NS) = $tick.foreachNode(get-ids(NS), NS) .
    op $tick.foreachNode : QidSet WorldState -> WorldState .
    eq $tick.foreachNode(empty, NS) = NS .
   ceq $tick.foreachNode((A, IDS), [ A S1 H1 W1 ]
                                   [ B S2 H2 W2 ]
                                   NS            )
     = $tick.foreachNode(IDS, [ A record-block-sent(S1, B) H1 W1 ]
                              [ B record-block-received(S2, A) (H2, take(intersection(H1, W2))) W2 ]
                              NS)
      if send-next-block-to(S1, could-send-to(H1, [ B S2 H2 W2 ] NS)) == B
      [print A " sent block to " B]
     .

    eq $tick.foreachNode((A, IDS), [ A S1 H1 W1 ]                                                                NS)
     = $tick.foreachNode(IDS,      [ A record-block-sent(S1, nobody) H1 W1 ]                                     NS)
       [owise]
     .

    op naive : -> Strategy                                              [ctor] .
    eq send-next-block-to(naive, (A, IDS)) = A                                 .
    eq send-next-block-to(naive,     IDS ) = nobody                    [owise] .
    eq record-block-sent    (naive, A) = naive .
    eq record-block-received(naive, A) = naive .

    op round-robin : QidList -> Strategy                                [ctor] .
    eq send-next-block-to(round-robin((A IDLIST)), (A, IDS)) = A               .
    eq send-next-block-to(round-robin((A IDLIST)), IDS)
     = send-next-block-to(round-robin((IDLIST)), IDS)                  [owise] .
    eq record-block-sent(round-robin((IDLIST1 A IDLIST2)), A)
     =                   round-robin((IDLIST1 IDLIST2 A))
     .
    eq record-block-sent(round-robin(IDLIST), nobody) = round-robin(IDLIST) .
    eq record-block-received(round-robin(IDLIST), A     ) = round-robin(IDLIST) .
    eq record-block-received(round-robin(IDLIST), nobody) = round-robin(IDLIST) .
    
endfm

reduce could-send-to(('p, 'q, 'r), ['a round-robin('b 'c) ('x, 'y, 'z) ('p, 'q, 'r)]) == 'a .
reduce could-send-to(('x, 'p, 'z), ['a round-robin('b 'c) ('x, 'y, 'z) ('p, 'q, 'r)]) == 'a .
reduce could-send-to(('x, 'y, 'z), ['a round-robin('b 'c) ('x, 'y, 'z) ('p, 'q, 'r)]) == empty .

reduce take(intersection(('a, 'b, 'c), ('b, 'c, 'd, 'e))) .
reduce take(intersection(('a, 'b, 'c), ('d, 'e))) == empty .

reduce tick(empty) .
          
reduce tick([ 'a naive ('x, 'y, 'z) ('p, 'q, 'r) ]
            [ 'b naive ('p, 'q, 'r) ('x, 'y, 'z) ]
           ) .

reduce tick([ 'a round-robin(('b 'c)) ('x, 'y, 'z) ('p, 'q, 'r) ]) .
reduce tick([ 'a round-robin(('b 'c)) ('x, 'y, 'z) ('p, 'q, 'r) ]
            [ 'b round-robin(('a 'c)) ('p, 'q, 'r) ('x, 'y, 'z) ]
           ) .
reduce tick([ 'a round-robin(('b 'c)) ('x, 'y, 'z) ('p, 'q, 'r) ]
            [ 'b round-robin(('a 'c)) ('x, 'y, 'z) ('p, 'q, 'r) ]
           ) .
```
