# Graph Report - property  (2026-11-17)

## Corpus Check
- cluster-only mode — file stats not available

## Summary
- 129 nodes · 215 edges · 7 communities (6 shown, 1 thin omitted)
- Extraction: 100% EXTRACTED · 0% INFERRED · 0% AMBIGUOUS · INFERRED: 1 edges (avg confidence: 0.85)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `a5247d1f`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- true_type
- false_type
- musCheckOperator.h
- Property
- musProperty.h
- MyClass
- property

## God Nodes (most connected - your core abstractions)
1. `MyClass` - 14 edges
2. `Property` - 14 edges
3. `dene` - 8 edges
4. `MyClass` - 6 edges
5. `MyClass` - 6 edges
6. `MyClass1` - 5 edges
7. `GetterInfo` - 3 edges
8. `has_left_shift_equal<T, U, void_t<decltype(declval<T&&>() <<= declval<U>())>>` - 2 edges
9. `has_right_shift_equal` - 2 edges
10. `has_right_shift_equal<T, U, void_t<decltype(declval<T&&>() >>= declval<U>())>>` - 2 edges

## Surprising Connections (you probably didn't know these)
- `MyClass` --references--> `Property`  [EXTRACTED]
  examples/example2.cpp → includes/musProperty.h
- `MyClass` --references--> `Property`  [EXTRACTED]
  examples/example3.cpp → includes/musProperty.h
- `MyClass` --references--> `Property`  [EXTRACTED]
  examples/example1.cpp → includes/musProperty.h
- `MyClass1` --references--> `Property`  [EXTRACTED]
  examples/example2.cpp → includes/musProperty.h

## Import Cycles
- None detected.

## Communities (7 total, 1 thin omitted)

### Community 0 - "true_type"
Cohesion: 0.05
Nodes (36): has_arrow_star<T, U, void_t<decltype(declval<T&&>().operator->*())>>, has_arrow<T, U, void_t<decltype(declval<T&&>().operator->())>>, has_assignment<T, U, void_t<decltype(declval<T&&>() = declval<U&&>())>>, has_bitwise_and_equal<T, U, void_t<decltype(declval<T&&>() &= declval<U>())>>, has_bitwise_and<T, U, void_t<decltype(declval<T&&>()& declval<U>())>>, has_bitwise_or_equal<T, U, void_t<decltype(declval<T&&>() |= declval<U>())>>, has_bitwise_or<T, U, void_t<decltype(declval<T&&>() | declval<U>())>>, has_bitwise_xor_equal<T, U, void_t<decltype(declval<T&&>() ^= declval<U>())>> (+28 more)

### Community 1 - "false_type"
Cohesion: 0.11
Nodes (18): has_arrow_star, has_bitwise_and_equal, has_bitwise_xor, has_bitwise_xor_equal, has_divide, has_divide_equal, has_function_call, has_greater_equal (+10 more)

### Community 2 - "musCheckOperator.h"
Cohesion: 0.11
Nodes (18): has_arrow, has_assignment, has_bitwise_and, has_bitwise_or, has_bitwise_or_equal, has_equal, has_greater, has_left_shift (+10 more)

### Community 3 - "Property"
Cohesion: 0.14
Nodes (8): MyClass1, valueProperty, operator==(), Property, GetterInfo, GetterInfo<VT(C::*)()>, GetterInfo<VT(C::*)() const>, GetterInfo<VT(C::*)() volatile>

### Community 4 - "musProperty.h"
Cohesion: 0.12
Nodes (5): MyClass, value_, valueProperty, MyClass, myProperty_

### Community 5 - "MyClass"
Cohesion: 0.16
Nodes (8): aabh(), dene, main(), MyClass, aaa, value_, valueProperty, valuePropertyaaa

## Knowledge Gaps
- **8 isolated node(s):** `valueProperty`, `value_`, `valueProperty`, `myProperty_`, `aaa` (+3 more)
  These have ≤1 connection - possible missing edges. (Counts symbols only; 29 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **1 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `Property` connect `Property` to `musProperty.h`, `MyClass`?**
  _High betweenness centrality (0.315) - this node is a cross-community bridge._
- **What connects `valueProperty`, `value_`, `valueProperty` to the rest of the system?**
  _8 weakly-connected nodes found - possible missing edges._
- **Should `true_type` be split into smaller, more focused modules?**
  _Cohesion score 0.05405405405405406 - nodes in this community are weakly interconnected._
- **Why does `MyClass` connect `MyClass` to `Property`?**
  _High betweenness centrality (0.149) - this node is a cross-community bridge._
- **Should `false_type` be split into smaller, more focused modules?**
  _Cohesion score 0.10526315789473684 - nodes in this community are weakly interconnected._
- **Why does `MyClass` connect `musProperty.h` to `Property`?**
  _High betweenness centrality (0.064) - this node is a cross-community bridge._
- **Should `musCheckOperator.h` be split into smaller, more focused modules?**
  _Cohesion score 0.10526315789473684 - nodes in this community are weakly interconnected._