asa-project1
===========

Project assignment for the Analysis and Synthesis of Algorithms course. 

LEIC-T # 2013/2014
www.tecnico.ulisboa.pt/ 

 The Problem
-
Nowadays there are many ways of sharing information (texts, images, videos, etc..) between people. Some people end up forming groups so that when any member of the group gets something, everyone in the group has access to this information through a sequence of shares.
Suppose that each person has a range of other people with whom he shares the receiving. Your task is classify people into groups for sharing, so that when a group of people gets something, all other will also receive.

 Input
-
The input file must contain information about the shares:

  - A line with the number of people N, and the number of shares P.
  
  - A list of P lines, where each line contains two integers u and v (separated by a blank space) which represent a share. Each line indicates that the person sharing what u get with v.
  
It is assumed that the identification of people and an integer between 1 and N.

 Output
-
The program should write to output the following information: 

  - A line with the number of maximums groups of people who share information; 
  
  - A line with the maximum size of the largest group of people who share information; 
  
  - A line with the number of maximums groups of people who share information only within the group;


Examples
-

- Input 1<br/>

  8 10 <br/>
  1 2 <br/>
  1 3 <br/>
  3 4 <br/>
  4 2 <br/>
  2 1 <br/>
  2 5 <br/>
  5 6 <br/>
  6 7 <br/>
  7 5 <br/>
  4 8 <br/>
  
- Output 1<br/>

  3 <br/>
  4 <br/>
  2 <br/>
  
- Input 2<br/>

  7 5 <br/>
  2 3 <br/>
  3 4 <br/>
  5 6 <br/>
  6 7 <br/>
  7 5 <br/>
  
- Output 2<br/>

  5 <br/>
  3 <br/>
  3 <br/>

===

README by [manuelalves](https://github.com/manuelalves) - https://github.com/manuelalves/asa-problem1/