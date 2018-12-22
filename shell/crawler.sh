#!/bin/bash

download_url()
{
    if `wget -O file.html $1 &> /dev/null`
    then
        echo "NOW EXPLORING URL : $1"
        echo "NOW EXPLORING URL : $1" >> output.txt
        search
        parse
    fi
}

search()
{
    echo "RESULT :" >> output.txt
    if grep $seq file.html >> output.txt 2> /dev/null;then
        echo -n -e "\e[32m" 
        echo "!!!!!!!!!!!!!! FOUND MATCHING SEQUENCE !!!!!!!!!!!!!!!!!"
        echo -n -e "\e[39m"
    fi
}

parse()
{
    grep "href=" file.html | grep -o "https://[^ ]*\"" | rev | cut -c 2- | rev >> urls.txt
    grep "href=" file.html | grep -o "http://[^ ]*\"" | rev | cut -c 2- | rev >> urls.txt
}


rm file.html &> /dev/null
rm urls.txt &> /dev/null
rm visited.txt &> /dev/null
rm output.txt &> /dev/null
touch visited.txt
touch urls.txt
touch output.txt

seq=$2

download_url $1
while read url; do
    
    if grep -w "$url" visited.txt &> /dev/null; then
        :
    else
        download_url "$url"    
        echo "$url" >> visited.txt
    fi
done < urls.txt
