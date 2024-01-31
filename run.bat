cd virtual_project


gitil delete
gitil init
gitil set -m "commmmmmit!" -s cmt
gitil set -m "coit!" -s cmt2
gitil set -m "commm!" -s cmt3
gitil replace -m "hello!" -s cmt2
@REM gitil remove -s cmt2

gitil add .
gitil status -p
gitil commit -s cmt2
@REM gitil branch br1
@REM gitil status -p
@REM gitil add .
@REM gitil commit -m "br1 1"
@REM gitil branch
@REM gitil checkout master

gitil test

cd ..
