# My try on making NITC compiler

The needed info for this grammar is given in the pdf file

## Bugs that I know that exist.

### Stack allocation while function run

 - for each function my argc is copied to BP-2 and BP-3 stack points
 - BP-3 should be the correct point
 - BP-2 is the space to store return value
 - But I am using the same register to push to get the empty space
 - While returing i copy the return value to BP-2 so argc gets removed
 - So technically my code is not breaking (yet)
