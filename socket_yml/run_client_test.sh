gcc -o client_yml_test test_client_func.c client_func_yml.c
rm -r result
mkdir result
./client_yml_test
