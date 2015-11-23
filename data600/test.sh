#!/bin/bash
int=595
while(($int <= 650))
do
	i=0
	sum=0
	while(($i < 10))
	do
		echo ${int}_${i}
		rep=0
		./graphGen ${int} > file${int}
		while(($rep < 10))
		 do
		  cat file${int} | ./a5-ece650 >> res${int}
		  let "rep++"
		 done
		let "i++"
	done	
	let "int=int+10"
done
