#include<bits/stdc++.h>
using namespace std;
int hits = 0,misses = 0;
void LRU_Algorithm(vector<uint32_t>&associate_cache,vector<int>&recent_used_array,uint32_t tagvalue)
{
	int minval = recent_used_array[0],minindex = 0;
	int maxval = recent_used_array[0];
	for(int i = 0;i<associate_cache.size();i++)
	{
		if(recent_used_array[i] < minval)
		{
			minval = recent_used_array[i];
			minindex = i;
		}
		else if( recent_used_array[i] >= maxval)
		{
			maxval = recent_used_array[i];
		}
	}
	//cout<<"minval and max val "<<minval<<" "<<maxval<<endl;
	associate_cache[minindex] = tagvalue;
	//for(int i = 0;i<associate_cache.size();i++) cout<<"assoc:"<<associate_cache[i]<<" ";
	//cout<<"\n";
	recent_used_array[minindex] = maxval+1;
	//for(int i = 0;i < recent_used_array.size();i++) cout<<recent_used_array[i]<<" ";
	//cout<<"\n";
}
void FIFO_Algorithm(vector<uint32_t>&associate_cache,uint32_t tagvalue,queue<int>&qu)
{
	if(qu.empty()) return;
	int index = qu.front();
	//cout<<"Index "<<index<<"\n";
	qu.pop();
	associate_cache[index] = tagvalue;
	qu.push(index);
}
void Associative_Cache_Memory(int blk_size,int cache_size,int add_bits,vector<uint32_t>&address_final,int operation)
{
	int cache_blocks = cache_size/blk_size;
	vector<uint32_t>associate_cache(cache_blocks,-1);
	vector<int>recent_used_array(cache_blocks,-1);
	queue<int>qu;
	for(int i = 0;i < 10000;i++)
	{
		uint32_t address_index = i % 25;
		uint32_t address = address_final[address_index];
		uint32_t offset_bits = log2(blk_size);
		//cout<<"offset_bits"<<offset_bits<<endl;
		uint32_t offset_val = address & (((uint32_t)pow(2,offset_bits)) - 1);
		uint32_t tag_bits = add_bits - offset_bits;
		uint32_t tag_value = address & (((uint32_t)(pow(2,tag_bits) - 1)) << offset_bits);
		tag_value = tag_value >> offset_bits;
		//cout<<tag_value<<" "<<offset_val<<" "<<address<<"\n";
		int flag = 0;
		for(int j = 0;j < associate_cache.size();j++)
		{
			if(associate_cache[j] == tag_value)
			{
				hits++;
				recent_used_array[j] = *max_element(recent_used_array.begin(),recent_used_array.end()) + 1;
				flag = 1;
				break;
			}
			else if(associate_cache[j] == -1)
			{
				//misses++;
				associate_cache[j] = tag_value;
				recent_used_array[j] = *max_element(recent_used_array.begin(),recent_used_array.end()) + 1;
				flag = 1;
				qu.push(j);
				break;
			}
		}
		if(flag == 0)
		{
			if(operation == 1)
			{
				LRU_Algorithm(associate_cache,recent_used_array,tag_value);
				misses++;
			}
			else
			{
				FIFO_Algorithm(associate_cache,tag_value,qu);
				misses++;
			}
		}
	}
}
void Set_Associative_Cache_Memory(int n_ways,int blk_size,int cache_size,int add_bits,vector<uint32_t>&address_final,int option)
{
	map<uint32_t,vector<uint32_t>>set_associate_cache;
	map<uint32_t,queue<int>>Queue;
	map<uint32_t,vector<int>>recent_used;
	map<uint32_t,int>index;
	//map<uint32_t,vector<int>>valid;
	uint32_t set_bits = log2(cache_size/(n_ways * blk_size));
	for(int i = 0;i<10000;i++)
	{
		uint32_t address_index = i % 25;
		uint32_t address = address_final[address_index];
		uint32_t offset_bits = log2(blk_size);
		//uint32_t set_bits = log2(n_ways);
		uint32_t tag_bits = add_bits - (offset_bits + set_bits);
		uint32_t offset_val = address & (((uint32_t)pow(2,offset_bits))-1);
		uint32_t set_value = (((uint32_t)pow(2,set_bits))-1) & (address >> offset_bits);
		uint32_t tag_value = address >> (set_bits + offset_bits);
		int flag =0,idx;
		for(int j = 0;j<set_associate_cache[set_value].size();j++)
		{
			if(set_associate_cache[set_value][j] == tag_value)
			{ 
				hits++; 
				flag = 1;
				int max_val = *max_element(recent_used[set_value].begin(),recent_used[set_value].end());
				recent_used[set_value][j] = max_val + 1;
				//idx = j;
				break;
			}
		}
		if(flag ==0 && set_associate_cache[set_value].size() < n_ways)
		{
			//misses++;
			set_associate_cache[set_value].push_back(tag_value);
			Queue[set_value].push(index[set_value]++);
			if(recent_used[set_value].size() == 0)
			{
				recent_used[set_value].push_back(0);
				//misses++;
			}
			else
			{
				int max_val = *max_element(recent_used[set_value].begin(),recent_used[set_value].end());
				recent_used[set_value].push_back(max_val + 1);
				//recent_used[set_value][idx] = max_val + 1;
				//misses++;
			}
		}
		else if( flag == 0 )
		{
			if(option == 1)
			{
				LRU_Algorithm(set_associate_cache[set_value],recent_used[set_value],tag_value);
				misses++;
			}
			else
			{
				FIFO_Algorithm(set_associate_cache[set_value],tag_value,Queue[set_value]);
				misses++;
			}
		
		}
	}
}
int main()
{
	map<uint32_t,unsigned char>address_store;
	vector<unsigned char>address{0x40,0x44,0x48,0x4C,0x70,0x74,0x78,0x7C,0x80,0x84,0x88,0x8C,0x90,0x94,0x98,0x9C,0x0,0x4,0x8,0xC,0x10,0x14,0x18,0x1C,0x20};
	vector<uint32_t>address_final;
	for(int i = 0;i < address.size();i++) {address_store[(uint32_t)address[i]] = address[i];address_final.push_back((uint32_t)address[i]);}
	int blk_size,cache_size,add_bits,choice,option,n_ways,setnum;
	cout << "Enter block size in words\n";
	cin >> blk_size;
	blk_size = blk_size * 4;
	cout<< "Enter cache size in words \n";
	cin >> cache_size;
	cache_size = cache_size * 4;
	cout<<"Enter address bits\n";
	cin>>add_bits;
	cout << "Enter your choice of type of cache\n1.Direct_Cache_Memory\n2.Associative_Cache_Memory\n3.Set_Associative_Cache_Memory\n";
	cin >> choice;
	switch(choice)
	{
		case 1 :	Set_Associative_Cache_Memory(1,blk_size,cache_size,add_bits,address_final,2); break;
		case 2 :	cout<<"Enter your option of replacement policy \n1. LRU \n2. FIFO\n";
					cin >> option;
					switch(option)
					{
						case 1 : Associative_Cache_Memory(blk_size,cache_size,add_bits,address_final,1); break;
						case 2 : Associative_Cache_Memory(blk_size,cache_size,add_bits,address_final,2); break;
					}
					break;
		case 3 :	cout<<"Enter set size (k) : k represents k-way set-Associative_Cache_Memory \n";
					cin>>n_ways;
					cout<<"Enter your option of replacement policy \n1. LRU \n2. FIFO\n";
					cin >> option;
					switch(option)
					{
						case 1 : Set_Associative_Cache_Memory(n_ways,blk_size,cache_size,add_bits,address_final,1); break;
						case 2 : Set_Associative_Cache_Memory(n_ways,blk_size,cache_size,add_bits,address_final,2); break;
					}
					break;
	}
	cout<<"hits: "<<hits<<" "<<"misses :"<<misses<<"\n";
	cout<<"The miss rate of cache is "<<((misses/10000.0) * 100);
	return 0;
}