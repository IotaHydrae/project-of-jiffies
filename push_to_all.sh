#!/bin/bash 

git add .
git commit -m "daily update."
git push -u origin master 
git push -u github master 

echo "PUSH DONE."
