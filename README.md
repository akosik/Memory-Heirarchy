# Memory-Hierarchy
##### Alec Kosik
## Analysis
####Design
My C code creates a buffer of random numbers between 0 and the size of the buffer, for a given length.  Each time a test is run
a buffer is created.  Once the buffer has been created the code then starts the timer and enters a loop in which it reads the element
at a random index (given by a call to `random() % size` before the loop starts) and then copies this element into a local variable.
The currently accessed element of the buffer becomes the index for the next access and so forth.  The loop runs 1000 times and then 
stops the timer.  The total running time is divided by the number of iterations to get the average time per memory access.  Since
the compiler keeps the local variable used for indexing in a register the only memory access performed is the one we want to measure.
However, there is a small overhead from running the loop, luckily this only includes one arithmetic operation instruction.

![](https://github.com/akosik/Memory-Hierarchy/blob/master/tests.png "")

#### Notes on this graph
As mentioned, I was accessing 8 byte integers instead of single bytes.  However, I have already accounted for the 8 bytes in each element
so that KB *actually* means KB and not K(64 bit integers).  All of the data was also run without a GUI to cut down on other processes
interfering.

####Graph for consistency

![](https://github.com/akosik/Memory-Hierarchy/blob/master/testslong.png "")

####Notes on this graph
As you can see, the data is relatively stable, even for strange step sizes (i.e. (times)1.2, rounded to the nearest integer).
This graph is also adjusted so that KB *actually* means KB.  Since this graph distinguishes between caches better (because of 
the smaller step size) we will focus on it in the three zoomed in cache graphs (though the first graph is nicer to look at).

#### L1 Cache Graph

![](https://github.com/akosik/Memory-Hierarchy/blob/master/L1.png "")

####Notes on this graph
L1 and L2 are close in speed and thus hard to differentiate between.  This graph shows the first increase in latency from the
previous.  On that graph, this increase is barely noticeable, but when zoomed in we can see that there is indeed an increase from
about 1.5-2.5ns to 2.5-4.5ns.  Thus the first cache should be about 2^5 in size.  At 2^5 we notice a sharp decrease in latency for the data
point immediately succeeding the 2^5 hash mark.  It seems that there is some sort of optimization that occurs when a cache starts to fill
up, as we will see similar dips in the next cache graphs.  For now we will look at a graph of prefetched data to support this claim.

#### Prefetched Graph

![](https://github.com/akosik/Memory-Hierarchy/blob/master/prefetched.png "")

####Notes on this graph
This graph has not been adjusted for the use of 64 bit elements and the data was deleted so I couldn't regraph it so the graph is actually translated 3 hash marks 
to the left (the first peak should actually be at 2^15).  After calculation we notice that 2^15 bytes is approximately the size
of our estimate for the L1 cache.  Thus this graph seems to provide evidence for the claim that optimizations do, in fact, occur
once a cache fills up.  Each data point was averaged over 100 runs.

#### L2 Cache Graph

![](https://github.com/akosik/Memory-Hierarchy/blob/master/L2.png "")

####Notes on this graph
Here we see another zoomed-in graph for L2.  This is the second increase in the original, small step graph under the title graph for consistency.
This increase, unlike the L1 to L2 increase, is visible from  the original graph.  We once again notice a small drop in latency at around 
2^8, after which latency starts to increase again until flattening out at around 10ns between 2^9 KB and 2^11 KB.  Thus we estimate the L2 cache to be
about 2^8 KB.

#### L3 Cache Graph

![](https://github.com/akosik/Memory-Hierarchy/blob/master/L3.png "")

####Notes on this graph
The left end of this plot begins with the flat section seen in the L2 cache graph.  Since there seem to be two increases in this graph
we posit that this is due to the fact that the L3 cache is shared between all of the cores.  This is in line with the lecture on the memory hierarchy
which gave 2 different times for the L3 cache.  To determine the size of the cache we look at the second increase which starts between
2^12 and 2^13 but changes slope between 2^13 and 2^14.  Thus the size of the L3 cache is estimated to be between 2^12 and 2^14 KB.

####Plot Discrepancies
The graph is neither monotonous nor a step function most likely because the lower caches are still being used even after they fill up.
For example, at 2^6, the L1 cache has presumably filled up but is still able to hold half of the buffer.  Thus at 2^6 there is roughly
a 50% chance of a cache hit in L1.  So we expect the latency to increase slightly even if the computer has started to use the L2 cache
because it becomes less and less likely that there will be a hit in the L1 cache.  For the L2 cache this effect is seen almost all the way until 
it, itself fills up but in the case of the L3 cache, a constancy of latentcy is observable because it is disproportionately larger than the ratio 
between the L2 and L1 caches.

###CPUID
An inline asm cpuid call (see cachesizes.c) begot the following cache sizes:

**L1: 32KB**

**L2: ~256KB**  
**Actually: 262KB**

**L3: 10485KB**

(see caches.txt for more info)

L1 and L2 were spot on from my estimates and L3 was in the range I predicted.  My graph seems to be a little off in terms of the size 
of the L3 cache, or at least does not give a definite value for the the size.  The start of the increase in latency around the size 
of the L3 cache underestimates the size of the L3 cache.  This may be due to the fact that L3 is shared across cores and thus can have multiple
latency values according to where each value is in the cache.

###Times
The latencies for each cache are more in line with the the times mentioned in the memory hierarchy lecture.  That is, the L2 cache
has latency around 3-5ns, the L1 cache is about 1-2ns, and the L3 cache spans 12-40 (the beginning of L3 is a little under its expected latency but when the likelihood of an L2 cache hit is factored in it works out quite well).  Main memory is also slightly faster than shown in the norvig article--60-90ns as opposed to 100ns--but right on when compared to the number in the memory hierarchy lecture.  I assume that caches have sped up since 1998, when the article was written, so I think the slide from class is more accurate.
