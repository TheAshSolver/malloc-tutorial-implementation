#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
//#define NDEBUG

#include<assert.h>
typedef struct block_metadata{
struct block_metadata *next;

size_t size;
int free;

int magic;

}block_metadata;




void *global_base = NULL;
//simple malloc
void* my_malloc(size_t size){
void *p = sbrk(0);
p = sbrk(size);
if(p==(void *)-1){
    return NULL;
}
return p;





}




size_t METASIZE = sizeof(block_metadata);

block_metadata *find_block(size_t size, block_metadata** last){
block_metadata *current = global_base;
while(current!=NULL&&(current->size<size||current->free==0)){
*last = current;
current=current->next;


}


return current;



}







block_metadata *request_size(size_t size, block_metadata *last){
block_metadata *block = sbrk(size+METASIZE);
if(block==(void *)-1){
    return NULL;
}

if(last !=NULL){
last->next = block;



}
block->free = 0;
block->size = size;
block->magic = 3;
block->next=NULL;
return block;








}





//complicatled malloc
//implement a mechanism to store size of block using metadata
void* proper_malloc(size_t size){
//check if there is a free block that has a size greater than requested. If yes, return a pointer to that itself. Else, request a b
// a new block of that size. We see that the implementation of the block itself is by that function which is far more important.

block_metadata *block;
if(global_base==NULL){
block =request_size(size, NULL);
global_base = block;
if(block==NULL){
    return NULL;
}


}
else{
block_metadata *last = global_base;
block = find_block(size, &last);
if(!block){
    block = request_size(size, last);
    if(block==NULL){
        return NULL;
    }





}
else{
    block->free=0;

}




}
return block+1;

}




void free1(void *ptr1){
    block_metadata *ptr = (block_metadata*) ptr1;
    (ptr-1)->free=1;
    (ptr-1)->magic =0;
    return;
}


void *realloc1(int *ptr, size_t size){
    block_metadata *ptr2 = (block_metadata*)ptr;
    ptr2--;
    if(ptr2->size>=size){
        return ptr2+1;
    }

    block_metadata *block = sbrk(size);
    if(block==(void *)-1){
        return NULL;
    }
    memcpy(block,ptr2, ptr2->size );
    block->size = size;    
    return block+1;
}

int main(){
int *a = proper_malloc(10);
block_metadata *b = (block_metadata*)a;
b--;
printf("%d\n",b->magic);

free1(a);
printf("%d\n", b->magic);
int *c = realloc1(a, 200);return block+1;
block_metadata *d = (block_metadata*)c;
d--;
printf("%d\n", d->size);
}



