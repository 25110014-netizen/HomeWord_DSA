AuctionHub Sorting Algorithms - Reflection

1. Real-World Justification for Each Algorithm

Task A - Insertion Sort (Live Auction Monitor):
Incoming auction bids arrive nearly sorted because each new bid typically increments slightly above the previous one. Insertion Sort runs in O(n) linear time on nearly-sorted inputs, requiring minimal element shifts and zero auxiliary memory overhead.

Task B - Selection Sort (Top-K Bid Finder):
When extracting only the top K highest bids from a small dataset, Selection Sort can locate the maximum element in each pass and place it directly into position in-place. For small arrays and small K, its minimal write operations and simplicity outperform complex heap structures.

Task C - Interchange Sort (Price Anomaly Validator):
For small validation batches (<= 50 records), the execution time difference between O(n^2) and O(n log n) is negligible (microseconds). Interchange Sort provides a straightforward, easy-to-audit comparison structure without recursion overhead.

Task D - Bubble Sort with Early Stop (Stabilization Detector):
By tracking swaps with a boolean flag, Bubble Sort terminates immediately after a single O(n) pass if the stream is already ordered. This makes it ideal for detecting whether price movements have stabilized without sorting the entire window unnecessarily.

Task E - Quick Sort (Full Historical Sorter):
End-of-day reporting requires sorting millions of historical records where O(n^2) algorithms become unusable. Quick Sort achieves O(n log n) average time complexity with strong CPU cache locality and in-place partitioning.

2. Measured Runtime of Quick Sort (100,000 Records)

Dataset size: 100,000 random Bid records (test_data/large.txt)
Measured runtime: ~25 ms

3. Bug Encountered and Diagnosis

Bug: In bubbleSortEarlyStop, using isStable = !swapped inside the outer loop caused the function to return true even on unsorted arrays because the final pass after finishing the sort always performs zero swaps.
Diagnosis and Fix: By reviewing the loop state transition, I realized isStable must strictly reflect whether zero swaps occurred on the first pass (i == 0). Updating the logic to set isStable = false when swaps happen during i == 0 fixed the false-positive stability detection.

4. LLM Usage Log

1. "Explain why insertion sort achieves O(n) time complexity on nearly-sorted bid streams." - Green Zone (Conceptual understanding).
2. "Compare Lomuto and Hoare partition schemes when using median-of-three pivot selection." - Green Zone (Algorithmic trade-offs).
3. "Review my early-stop loop logic: why does my flag evaluate to true on the final pass of a reversed array?" - Green Zone (Debugging self-written logic).

5. Worst-Case Complexity of Quick Sort with Median-of-Three

Yes, the worst-case time complexity of Quick Sort with median-of-three pivot selection is still O(n^2). While median-of-three eliminates the worst-case behavior on already-sorted or reverse-sorted inputs, an adversarial input (such as a median-of-three killer sequence) can still force the chosen pivot to consistently be the second-smallest or second-largest element at every recursive step, degrading the partition tree to linear depth.
