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

bool is_odd(const void *num) {
    return (*(int *)num) % 2 != 0;
}

bool cc_array_filter_mut_test(){
    CC_Array* originarray;

    ASSERT_CC_OK(cc_array_new(&originarray));
    ASSERT_EQ(CC_ERR_OUT_OF_RANGE, cc_array_filter_mut(originarray, NULL));
    ASSERT_CC_OK(cc_array_add(originarray, (void*) 1));
    ASSERT_CC_OK(cc_array_add(originarray, (void*) 2));
    ASSERT_CC_OK(cc_array_add(originarray, (void*) 3));
    ASSERT_CC_OK(cc_array_add(originarray, (void*) 4));

    //pode estar dando erro aqui
    // Linha abaixo não é executada
    ASSERT_EQ(9, -1);
    cc_array_filter_mut(&originarray, is_odd); //deve ser dentro de um ASSERT_CC_OK


    ASSERT_EQ(2, cc_array_size(originarray));
}   

bool cc_array_filter_test(){
    CC_Array* originarray;
    CC_Array* filteredarray;
    CC_Array* filteredarray2;

    ASSERT_CC_OK(cc_array_new(&originarray));
    ASSERT_CC_OK(cc_array_new(&filteredarray));

    ASSERT_EQ(CC_ERR_OUT_OF_RANGE, cc_array_filter(originarray, NULL, &filteredarray));

    ASSERT_CC_OK(cc_array_add(originarray, (void*) 2));
    ASSERT_CC_OK(cc_array_add(originarray, (void*) 4));
    
    //daqui para baixo não é executado
    ASSERT_CC_OK(cc_array_filter(originarray, is_odd, &filteredarray2));
    ASSERT_EQ(9, -1);
    ASSERT_EQ(0,(int)cc_array_size(filteredarray2));

    ASSERT_CC_OK(cc_array_add(originarray, (void*) 1));
    ASSERT_CC_OK(cc_array_add(originarray, (void*) 3));

    ASSERT_CC_OK(cc_array_filter(originarray, is_odd, &filteredarray));
    ASSERT_EQ(2,(int)cc_array_size(filteredarray));
    ASSERT_NEQ(3,(int)cc_array_size(filteredarray));

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

void sum(void *a, void *b, void *result) {
    if (b == NULL) {
        *(int*)result = *(int*)a;
    } else {
        *(int*)result = *(int*)a + *(int*)b;
    }
}

bool c_array_reduce_teste(){
    CC_Array* originArray;
    int result = 0;

    ASSERT_CC_OK(cc_array_new(&originArray));

    cc_array_reduce(originArray, sum, &result);
    ASSERT_EQ(0, result);
    
    ASSERT_CC_OK(cc_array_add(originArray, (void*) 1));

    cc_array_reduce(originArray, NULL, &result);
    ASSERT_EQ(9, -1);
    ASSERT_EQ(1, result);

    ASSERT_CC_OK(cc_array_add(originArray, (void*) 2));

    // cc_array_reduce(originArray, sum, &result);
    // ASSERT_EQ(3, result);
    // ASSERT_NEQ(2, result);

    ASSERT_CC_OK(cc_array_add(originArray, (void*) 3));
    ASSERT_CC_OK(cc_array_add(originArray, (void*) 4));
    
    // cc_array_reduce(originArray,sum, &result);
    // ASSERT_EQ(10, result);

    // ASSERT_EQ(9, -1);

    return true; 
}

test_t TESTS[] = {
    &cc_array_subarray_test,
    &cc_array_copy_shallow_test,
    &cc_array_filter_mut_test,
    &cc_array_filter_test,
    &cc_array_reverse_test,
    &cc_array_trim_capacity_test,
    &c_array_reduce_teste,
    NULL
};
