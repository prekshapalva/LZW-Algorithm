## "LZW Algorithm" - FS Mini Project (VTU - Vth semester)

The "Lempel-Ziv-Welch Algorithm on text file" is developed and implemented using C++. This project has been submitted as a part of my VI semester File Structures Mini Project (18ISL67) of Bachelor of Engineering in Information Science & Engineering for the year 2021-22.

### :one: What is LZW?
Lempel–Ziv–Welch (LZW) Algorithm is a greedy lossless compression algorithm created by Abraham Lempel, Jacob Ziv and Terry Welch. The algorithm is simple to implement and has the potential for very high throughput in hardware implementations. Uncompressed data can take up a lot of space, which is not good for limited hard drive space and internet download speeds. This algorithm is typically used in GIF and optionally in PDF and TIFF. The GIF picture format uses the compress algorithm, which is a commonly used Unix file compression tool. In order to preserve data space, the Idea relies on recurring patterns. LZW is the most popular method for general purpose data compression because of its ease of use and adaptability. LZW is a type of LZ77 lossless compression. LZW compression works by reading a sequence of symbols, grouping the symbols into strings, and converting the strings into codes. Because the codes take up less space than the strings they replace, we get compression. The idea of the compression algorithm is the following as the input data is being processed, a dictionary keeps a correspondence between the longest encountered words and a list of code values.

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
  <img src="">
 </p>

### :four: Overwiew of YOLO object detection algorithm
In progress

### :five: Conclusion
In progress. Thank you!
