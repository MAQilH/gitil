cd virtual_project


gitil delete
gitil init
gitil set -m "commmmmmit!" -s cmt
gitil set -m "coit!" -s cmt2
gitil set -m "commm!" -s cmt3
gitil replace -m "hello!" -s cmt2

@REM gitil add .
@REM gitil status -p
@REM gitil commit -s cmt2
gitil add .
gitil commit -m "mohammad aqil hafezi"
gitil add xxx
gitil commit -s cmt2
@REM gitil add .
@REM gitil commit -s cmt3

gitil log -search a*il


@REM gitil branch br1
@REM gitil status -p
@REM gitil add .
@REM gitil commit -m "br1 1"
@REM gitil branch
@REM gitil checkout master

@REM gitil test "a*dr" "addedr" 

cd ..
