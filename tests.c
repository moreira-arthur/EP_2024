#include "tests.h"
#include "lib.h"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

bool cc_array_subarray_test(){
    CC_Array* originarray;
    CC_Array* subarray;
    CC_Array* subarrayfinal;

    cc_array_new(&originarray);
    cc_array_new(&subarray);
    cc_array_new(&subarrayfinal);
    
    //Test1: b>e
    ASSERT_EQ(CC_ERR_INVALID_RANGE, cc_array_subarray(originarray, 1, 0, &subarray));
    //Test2: e==ar->size
    ASSERT_EQ(CC_ERR_INVALID_RANGE, cc_array_subarray(originarray, 0, 0, &subarray));


    cc_array_add(originarray, (void*) 1);
    //Test3: b>e
    ASSERT_EQ(CC_ERR_INVALID_RANGE, cc_array_subarray(originarray, 1, 0, &subarray));
    //Test4: e==ar->size
    ASSERT_EQ(CC_ERR_INVALID_RANGE, cc_array_subarray(originarray, 0, 1, &subarray));
	
    //Test5: e>ar->size
    ASSERT_EQ(CC_ERR_INVALID_RANGE, cc_array_subarray(originarray, 0, 2, &subarray));

    cc_array_add(originarray, (void*) 2);
    cc_array_add(originarray, (void*) 3);
    cc_array_add(originarray, (void*) 4);

    //Test6: b=e
    ASSERT_CC_OK(cc_array_subarray(originarray, 1, 1, &subarray));

    //Test6: all ok
    ASSERT_CC_OK(cc_array_subarray(originarray, 1, 3, &subarray));
    ASSERT_EQ(3, cc_array_size(subarray));

    ASSERT_CC_OK(cc_array_subarray(originarray, 0, ((int)cc_array_size(originarray) - 1), &subarrayfinal));

    //Test7: check if the originarray is equal to the subarrayfinal 
    for(int i=0; i<cc_array_size(originarray); i++){
        void* get_result_subarrayfinal;
        void* get_result_originarray;
        cc_array_get_at(originarray, i, &get_result_originarray);
        cc_array_get_at(subarrayfinal, i, &get_result_subarrayfinal);
        ASSERT_EQ((int) get_result_originarray, (int) get_result_subarrayfinal);
    }

    //Test8: check if the size of the originarray is equal to the size of the subarrayfinal
    ASSERT_EQ((int)cc_array_size(originarray), (int)cc_array_size(subarrayfinal));

    cc_array_destroy(originarray);
    cc_array_destroy(subarray);

    return true;
}


bool cc_array_copy_shallow_test(){
    CC_Array* originarray;
    CC_Array* shallowarray;
    ASSERT_CC_OK(cc_array_new(&originarray));
    ASSERT_CC_OK(cc_array_new(&shallowarray)); 

    //Test0: all empty
    ASSERT_CC_OK(cc_array_copy_shallow(originarray, &shallowarray));
    
    ASSERT_CC_OK(cc_array_add(originarray, (void*) 1));
    ASSERT_CC_OK(cc_array_add(originarray, (void*) 2));
    ASSERT_CC_OK(cc_array_add(originarray, (void*) 3));

    //Test1: all ok
    ASSERT_CC_OK(cc_array_copy_shallow(originarray, &shallowarray));

    for(int i=0; i<cc_array_size(originarray); i++){
        void* get_result_shallowarray;
        void* get_result_originarray;
        ASSERT_CC_OK(cc_array_get_at(originarray, i, &get_result_originarray));
        ASSERT_CC_OK(cc_array_get_at(shallowarray, i, &get_result_shallowarray));
        ASSERT_EQ((int) get_result_originarray, (int) get_result_shallowarray);
    }
    ASSERT_EQ((int)cc_array_size(originarray), (int)cc_array_size(shallowarray));
    return true;
}





//ar->size == 1 é equivalente um array de tamanho 1 e com 1 elemento invertido é igual a ele mesmo
//ar->size <= 0 é equivalente a um array vazio, que ao ser invertido continua vazio e nunca tera tamanho negativo
//ar-size <0 é equivalente a um array com tamanho negativo, que não existe, portanto sempre ira executar todo o código da função e nao é testável
bool cc_array_reverse_test(){
    CC_Array* array;
    CC_Array* arraycopy;
    CC_Array* controlarray;

    ASSERT_CC_OK(cc_array_new(&array));

    ASSERT_CC_OK(cc_array_add(array, (void*) 1));
    ASSERT_CC_OK(cc_array_add(array, (void*) 2));
    ASSERT_CC_OK(cc_array_add(array, (void*) 3));
    ASSERT_CC_OK(cc_array_add(array, (void*) 4));

    ASSERT_CC_OK(cc_array_copy_shallow(array, &arraycopy));
    cc_array_reverse(array);

    for(int i=0; i<4; i++){
        void* get_result_array;
        ASSERT_CC_OK(cc_array_get_at(array, i, &get_result_array));
        ASSERT_EQ((int) get_result_array, (int) 4-i);	
    }

    cc_array_reverse(array);

    return true;
}

// os outros dois mutantes dessa função não são testáveis pois a condição size>capacity nunca é atingida, e quando a size > capacity, a função retorna CC_OK e não há como testar se a função está retornando CC_OK ou não
bool cc_array_trim_capacity_test(){
    CC_Array* array;
    ASSERT_CC_OK(cc_array_new(&array));

    ASSERT_CC_OK(cc_array_add(array, (void*) 1));
    ASSERT_CC_OK(cc_array_add(array, (void*) 2));
    ASSERT_CC_OK(cc_array_add(array, (void*) 3));
    ASSERT_CC_OK(cc_array_add(array, (void*) 4));

    ASSERT_CC_OK(cc_array_trim_capacity(array));
        ASSERT_EQ(4, (int)cc_array_capacity(array));

    ASSERT_CC_OK(cc_array_add(array, (void*) 5));
    ASSERT_CC_OK(cc_array_add(array, (void*) 6));
    ASSERT_CC_OK(cc_array_remove_last(array, NULL));
    ASSERT_CC_OK(cc_array_trim_capacity(array));

    ASSERT_EQ(5, (int)cc_array_capacity(array));
    cc_array_remove_all(array);
    ASSERT_CC_OK(cc_array_trim_capacity(array));
    ASSERT_EQ(0, (int)cc_array_capacity(array));

    return true;
}



bool teste_cc_array_add(){
    CC_Array* a;
    void* get_result;
    ASSERT_CC_OK(cc_array_new(&a));
    ASSERT_EQ(8, cc_array_capacity(a));

    ASSERT_CC_OK(cc_array_add(a, (void*) 1));
    ASSERT_CC_OK(cc_array_trim_capacity(a));
    ASSERT_EQ(1, cc_array_capacity(a));
    
    ASSERT_CC_OK(cc_array_add(a, (void*) 2));
    ASSERT_EQ(2, cc_array_capacity(a));

    ASSERT_CC_OK(cc_array_add(a, (void*) 3));
    ASSERT_CC_OK(cc_array_trim_capacity(a));
    ASSERT_EQ(3, cc_array_capacity(a));
    ASSERT_EQ(3, cc_array_size(a));

    ASSERT_CC_OK(cc_array_remove_last(a, &get_result));
    ASSERT_EQ(3, (int) get_result);
    ASSERT_EQ(3, cc_array_capacity(a));

    ASSERT_CC_OK(cc_array_remove_last(a, &get_result));
    ASSERT_EQ(2, (int) get_result);
    ASSERT_EQ(3, cc_array_capacity(a));

    ASSERT_CC_OK(cc_array_add(a, (void*) 2));
    ASSERT_EQ(2, cc_array_size(a));
    ASSERT_EQ(3, cc_array_capacity(a));
    ASSERT_CC_OK(cc_array_add(a, (void*) 3));
    ASSERT_EQ(3, cc_array_size(a));
    ASSERT_EQ(3, cc_array_capacity(a));

    return true;
}

bool teste_cc_array_new_conf(){
    CC_Array* a;
    CC_Array* b;
    CC_ArrayConf confa, confb;

    cc_array_conf_init(&confa);
    confa.exp_factor = 1;
    confa.capacity = 2;
    ASSERT_CC_OK(cc_array_new_conf(&confa, &a));
    ASSERT_EQ(2, cc_array_capacity(a));
    ASSERT_CC_OK(cc_array_add(a, (void*) 1));
    ASSERT_EQ(2, cc_array_capacity(a));
    ASSERT_CC_OK(cc_array_trim_capacity(a));
    ASSERT_EQ(1, cc_array_capacity(a));
    ASSERT_CC_OK(cc_array_add(a, (void*) 1));
    ASSERT_CC_OK(cc_array_add(a, (void*) 2));
    ASSERT_CC_OK(cc_array_add(a, (void*) 3));
    ASSERT_CC_OK(cc_array_add(a, (void*) 4));
    ASSERT_EQ(8, cc_array_capacity(a));
    
    cc_array_conf_init(&confb);
    confb.exp_factor = 6;
    confb.capacity = 2;
    ASSERT_CC_OK(cc_array_new_conf(&confb, &b));
    ASSERT_EQ(2, cc_array_capacity(b));
    ASSERT_CC_OK(cc_array_add(b, (void*) 1));
    ASSERT_EQ(2, cc_array_capacity(b));
    ASSERT_CC_OK(cc_array_trim_capacity(b));
    ASSERT_EQ(1, cc_array_capacity(b));
    ASSERT_CC_OK(cc_array_add(b, (void*) 1));
    ASSERT_CC_OK(cc_array_add(b, (void*) 2));
    ASSERT_CC_OK(cc_array_add(b, (void*) 3));
    ASSERT_CC_OK(cc_array_add(b, (void*) 4));
    ASSERT_EQ(6, cc_array_capacity(b));

    cc_array_destroy(a);
    return true;
}

bool test_iter_next(){
    CC_Array* a;
    CC_ArrayIter iter;
    void* get_result;

    ASSERT_CC_OK(cc_array_new(&a));
    ASSERT_CC_OK(cc_array_add(a, (void*) 1));
    ASSERT_CC_OK(cc_array_add(a, (void*) 2));
    ASSERT_CC_OK(cc_array_add(a, (void*) 3));
    ASSERT_CC_OK(cc_array_add(a, (void*) 4));
    ASSERT_EQ(4, cc_array_size(a));

    // nao passa daqui

    cc_array_iter_init(&iter, a);
    iter.index = 0;
    ASSERT_CC_OK(cc_array_iter_next(&iter, &get_result));
    ASSERT_EQ(1, (int) get_result);
    ASSERT_EQ(0, (int) cc_array_iter_index(&iter));
    ASSERT_CC_OK(cc_array_iter_next(&iter, &get_result));
    ASSERT_EQ(2, (int) get_result);
    ASSERT_EQ(1, (int) cc_array_iter_index(&iter));
    ASSERT_CC_OK(cc_array_iter_next(&iter, &get_result));
    ASSERT_EQ(3, (int) get_result);
    ASSERT_EQ(2, (int) cc_array_iter_index(&iter));
    iter.index = 4;
    ASSERT_EQ(CC_ITER_END,cc_array_iter_next(&iter, &get_result));
    iter.index = 7;
    ASSERT_EQ(CC_ITER_END,cc_array_iter_next(&iter, &get_result));
    // ASSERT_FAIL();

    return true;
}

bool teste_cc_array_remove_at(){
    CC_Array* a;
    void* get_result;

    ASSERT_CC_OK(cc_array_new(&a));
    ASSERT_CC_OK(cc_array_add(a, (void*) 1));
    ASSERT_CC_OK(cc_array_add(a, (void*) 2));
    ASSERT_CC_OK(cc_array_add(a, (void*) 3));
    ASSERT_CC_OK(cc_array_add(a, (void*) 4));
    ASSERT_EQ(4, cc_array_size(a));
    ASSERT_EQ(8, cc_array_capacity(a));
    ASSERT_CC_OK(cc_array_trim_capacity(a));
    ASSERT_EQ(4, cc_array_capacity(a));

    ASSERT_CC_OK(cc_array_remove_at(a, 3, &get_result));
    ASSERT_EQ(4, (int) get_result);
    ASSERT_EQ(3, cc_array_size(a));
    ASSERT_EQ(4, cc_array_capacity(a));

    ASSERT_CC_OK(cc_array_remove_at(a, 1, &get_result));
    ASSERT_EQ(2, (int) get_result);
    ASSERT_EQ(2, cc_array_size(a));
    ASSERT_EQ(4, cc_array_capacity(a));

    // 1 3

    ASSERT_CC_OK(cc_array_add(a, (void*) 5));
    ASSERT_EQ(3, cc_array_size(a));
    ASSERT_EQ(4, cc_array_capacity(a));

    ASSERT_CC_OK(cc_array_remove_at(a, 0, &get_result));
    ASSERT_EQ(1, (int) get_result);
    ASSERT_EQ(2, cc_array_size(a));
    ASSERT_EQ(4, cc_array_capacity(a));
    ASSERT_EQ(CC_ERR_OUT_OF_RANGE, cc_array_remove_at(a, 5, &get_result));
    ASSERT_EQ(CC_ERR_OUT_OF_RANGE, cc_array_remove_at(a, 2, &get_result));
    ASSERT_CC_OK(cc_array_remove_at(a, 1, &get_result));
    ASSERT_EQ(5, (int) get_result);
    ASSERT_EQ(1, cc_array_size(a));
    ASSERT_EQ(4, cc_array_capacity(a));

    ASSERT_CC_OK(cc_array_remove_at(a, 0, &get_result));
    ASSERT_EQ(3, (int) get_result);
    ASSERT_EQ(0, cc_array_size(a));
    ASSERT_EQ(4, cc_array_capacity(a));

    return true;
}

void sum(void *a, void *b, void *result) {
    int *n_a = (int*)a;
    int *n_b = (int*)b;
    int *n_result = (int*)result;

    if (b == NULL) {
        *n_result = *n_a;
    } else {
        *n_result = *n_a + *n_b;
    }
}


bool teste_cc_array_reduce() {
    CC_Array *ar;
    ASSERT_CC_OK(cc_array_new(&ar));
    int result = 0;

    cc_array_reduce(ar, sum, &result);
    ASSERT_EQ(0, result); // empty array

    int dado_1 = 20;
    ASSERT_CC_OK(cc_array_add(ar, &dado_1));

    result = 0;
    cc_array_reduce(ar, sum, &result);
    ASSERT_EQ(20, result);

    int dado_2 = 21;
    int dado_3 = 23;
    ASSERT_CC_OK(cc_array_add(ar, &dado_2));
    ASSERT_CC_OK(cc_array_add(ar, &dado_3));

    result = 0;
    cc_array_reduce(ar, sum, &result);
    ASSERT_EQ(64, result);

    cc_array_destroy(ar);
    return true;
}

bool is_odd(const void *num) {
    return (*(int *)num) % 2 != 0;
}

bool teste_cc_array_filter_mut(){
    CC_Array* originarray;

    ASSERT_CC_OK(cc_array_new(&originarray));
    ASSERT_EQ(CC_ERR_OUT_OF_RANGE, cc_array_filter_mut(originarray, NULL));
    int dado_1 = 20;
    int dado_2 = 21;
    int dado_3 = 22;
    int dado_4 = 23;

    ASSERT_CC_OK(cc_array_add(originarray, &dado_1));
    ASSERT_CC_OK(cc_array_add(originarray, &dado_2));
    ASSERT_CC_OK(cc_array_add(originarray, &dado_3));
    ASSERT_CC_OK(cc_array_add(originarray, &dado_4));
    
    ASSERT_CC_OK(cc_array_filter_mut(originarray, is_odd));


    ASSERT_EQ(2, cc_array_size(originarray));

    return true;
}   

bool teste_cc_array_filter(){
    CC_Array* originarray;
    CC_Array* filteredarray;

    ASSERT_CC_OK(cc_array_new(&originarray));
    ASSERT_CC_OK(cc_array_new(&filteredarray));

    ASSERT_EQ(CC_ERR_OUT_OF_RANGE, cc_array_filter(originarray, NULL, &filteredarray));


    int dado_1 = 20;
    int dado_2 = 22;
    ASSERT_CC_OK(cc_array_add(originarray, &dado_1));
    ASSERT_CC_OK(cc_array_add(originarray, &dado_2));

    filteredarray = NULL;
    ASSERT_CC_OK(cc_array_new(&filteredarray));

    ASSERT_CC_OK(cc_array_filter(originarray, is_odd, &filteredarray));
    ASSERT_EQ(0,(int)cc_array_size(filteredarray));

    return true;

}

bool teste_cc_array_remove_at_new(){
    CC_Array* array;
    ASSERT_CC_OK(cc_array_new(&array));

    int dado_1 = 21;
    int dado_2 = 22;
    int dado_3 = 23;

    ASSERT_CC_OK(cc_array_add(array, &dado_1));
    ASSERT_CC_OK(cc_array_add(array, &dado_2));
    ASSERT_CC_OK(cc_array_add(array, &dado_3));
    ASSERT_CC_OK(cc_array_trim_capacity(array));

    void* removed;
    void* lookup;
    ASSERT_CC_OK(cc_array_remove_at(array, 2, &removed));
    ASSERT_EQ(&dado_3, removed);

    ASSERT_EQ(cc_array_get_at(array, 2, &lookup), CC_ERR_OUT_OF_RANGE);
    ASSERT_EQ(cc_array_size(array), 2);

    ASSERT_CC_OK(cc_array_get_at(array, 0, &lookup));
    ASSERT_EQ(&dado_1, lookup);

    ASSERT_CC_OK(cc_array_get_at(array, 1, &lookup));
    ASSERT_EQ(&dado_2, lookup);

    ASSERT_CC_OK(cc_array_remove_at(array, 1, &removed));
    ASSERT_EQ(&dado_2, removed);

    ASSERT_EQ(cc_array_size(array), 1);

    ASSERT_CC_OK(cc_array_get_at(array, 0, &lookup));
    ASSERT_EQ(&dado_1, lookup);

    ASSERT_CC_OK(cc_array_remove_at(array, 0, &removed));
    ASSERT_EQ(&dado_1, removed);

    ASSERT_EQ(cc_array_size(array), 0);


    ASSERT_CC_OK(cc_array_add(array, &dado_1));
    ASSERT_CC_OK(cc_array_add(array, &dado_2));
    ASSERT_CC_OK(cc_array_add(array, &dado_3));

    ASSERT_CC_OK(cc_array_remove_at(array, 1, &removed));
    ASSERT_EQ(&dado_2, removed);

    ASSERT_EQ(cc_array_size(array), 2);

    ASSERT_CC_OK(cc_array_get_at(array, 0, &lookup));
    ASSERT_EQ(&dado_1, lookup);

    ASSERT_CC_OK(cc_array_get_at(array, 1, &lookup));
    ASSERT_EQ(&dado_3, lookup);

    cc_array_destroy(array);    

    return true;
}

//Não terminou todos os casos ainda 
bool test_cc_array_zip_iter_add(){
    CC_Array *ar1, *ar2;
    cc_array_new(&ar1);
    cc_array_new(&ar2);

    int dado_1 = 21;
    int dado_2 = 22;
    int dado_3 = 23;

    ASSERT_CC_OK(cc_array_add(ar1, &dado_1));
    ASSERT_CC_OK(cc_array_add(ar1, &dado_2));
    ASSERT_CC_OK(cc_array_add(ar1, &dado_3));
    ASSERT_CC_OK(cc_array_trim_capacity(ar1));

    ASSERT_CC_OK(cc_array_add(ar2, &dado_1));
    ASSERT_CC_OK(cc_array_add(ar2, &dado_2));
    ASSERT_CC_OK(cc_array_add(ar2, &dado_3));
    ASSERT_CC_OK(cc_array_trim_capacity(ar2));


    CC_ArrayZipIter iter;
    cc_array_zip_iter_init(&iter, ar1, ar2);


    int n_element_1 = 1;
    int n_element_2 = 2;

    ASSERT_CC_OK(cc_array_zip_iter_add(&iter, &n_element_1, &n_element_2));


    void *lookup_1, *lookup_2;
    
    ASSERT_CC_OK( cc_array_get_at(ar1, 0, &lookup_1));
    ASSERT_CC_OK( cc_array_get_at(ar2, 0, &lookup_2));
    ASSERT_EQ(4, cc_array_size(ar1));
    ASSERT_EQ(4, cc_array_size(ar2));
    ASSERT_EQ(1, *((int *)lookup_1));
    ASSERT_EQ(2, *((int *)lookup_2));

    cc_array_destroy(ar1);
    cc_array_destroy(ar2);

    return true;
}

test_t TESTS[] = {
    &cc_array_subarray_test,
    &cc_array_copy_shallow_test,
    &cc_array_reverse_test,
    &cc_array_trim_capacity_test,
    &teste_cc_array_new_conf,
    &teste_cc_array_add,
    NULL
};
