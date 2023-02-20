## "LZW Algorithm" - FS Mini Project (VTU - VIth semester)

The "Lempel-Ziv-Welch Algorithm on text file" is developed and implemented using C++. This project has been submitted as a part of my VI semester File Structures Mini Project (18ISL67) of Bachelor of Engineering in Information Science & Engineering for the year 2021-22.

### :one: What is LZW?
The Lempel–Ziv–Welch (LZW) Algorithm is a greedy lossless compression algorithm developed by Abraham Lempel, Jacob Ziv, and Terry Welch. It is a simple algorithm to implement and is capable of very high throughput in hardware implementations. The size of uncompressed data can be large, which is not good for limited hard drive space and internet download speeds. LZW is the most popular method for general purpose data compression because of its ease of use and adaptability. LZW is a type of LZ77 lossless compression. LZW compression works by reading a sequence of symbols, grouping the symbols into strings, and converting the strings into codes. Because the codes take up less space than the strings they replace, we get compression. 

### :two: Specifications
Hardware Specification<br>
• Processor : Intel(R) Core(TM) i5-8265U CPU @ 1.60GHz 1.80 GHz<br>
• RAM : 8.00 GB<br>
• Hard Disk : 1TB<br>
• Input Device : Standard keyboard and Mouse<br>
• Output Device : Monitor<br>

Software Specification<br>
• Programming Language : C++<br>
• IDE : Visual Studio code (version 1.69)<br>

### :three: Architecture Diagram
The user enters the input in the form of text file. This input file is sent to the Encoding module to encode the data. Further, the encoded file with the encoded file is displayed before being sent to the Decoding module. In the Decoding module, the encoded file is decoded back to the oringinal input text file. Both, encoded file and decoded file along with the input file is displayed to the user along with the file size.
<p align=center>
  <img src="https://github.com/prekshapalva/LZW-Algorithm/blob/24047fd4f18b26d9c688fbe9584c844fd39e2b2a/LZWAlgorithm/architecturediagram.jpg" width=700>
 </p>

### :four: Pseudocode of LZW algorithm
ENCODING : 
````
  1     Initialize table with single character strings
  2     P = first input character
  3     WHILE not end of input stream
  4          C = next input character
  5          IF P + C is in the string table
  6            P = P + C
  7          ELSE
  8            output the code for P
  9          add P + C to the string table
  10           P = C
  11         END WHILE
  12    output code for P
 ````
 
DECODING :
````
1    Initialize table with single character strings
2    OLD = first input code
3    output translation of OLD
4    WHILE not end of input stream
5        NEW = next input code
6        IF NEW is not in the string table
7               S = translation of OLD
8               S = S + C
9       ELSE
10              S = translation of NEW
11       output S
12       C = first character of S
13       OLD + C to the string table
14       OLD = NEW
15   END WHILE
````

### :five: Conclusion
This project demonstrates LZW Algorithm and its two parts - the encoding algorithm, which converts strings to integer codes, and the decoding algorithm, which does the opposite. Thank you!
