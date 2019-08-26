#include <stdio.h>


struct board get_new_board(int num_blocks_w, int num_blocks_h, int block_size_w, int block_size_h,int fixed_cells) {
    struct board tmp;
    int width, height,i ,j;
    
    
    
    tmp.NUM_BLOCKS_W = num_blocks_w ;
    tmp.NUM_BLOCKS_H = num_blocks_h ;
    tmp.BLOCK_SIZE_W = block_size_w ;
    tmp.BLOCK_SIZE_H = block_size_h ;
    
    width = num_blocks_w * block_size_w ;
    height = num_blocks_h * block_size_h ;
    
    tmp.WIDTH = width;
    tmp.HEIGHT = height;
    
    tmp.FIXED_CELLS_NUM = fixed_cells;
    
    
    tmp.CELLS = (int**)malloc(height * sizeof(int *));
    tmp.CELLS_FIXED = (int**)malloc(height * sizeof(int *));
    for (i = 0; i < height; ++i)
    {
        tmp.CELLS[i] = (int*) malloc(width * sizeof(int));
        tmp.CELLS_FIXED[i] = (int*) malloc(width * sizeof(int));
    }
    
    
    return tmp;
}
