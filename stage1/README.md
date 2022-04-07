# Identify Math Expressions

 - In this stage we will try to identify +, (, )
 - Make the Abstract Syntax tree for that particular expression.

```
3-2

  -
 / \
3   2


(3+2)+(5+6)

     +
   /   \
  +     +
 / \   / \
3   2  5  6
```

 - Finally extend the code to identify *, /, - also. with correct associativity rules
 - `./nitc` will take in input like `3+2*5-4` and give you xsm machine code as `stdout`
