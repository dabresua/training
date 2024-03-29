#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <cstring>
#include <cstdlib>
#include <string>

/**
 * @brief test for using boost library shared_memory_object
 *        Uses a memory region to share info between processes
 *        Compile with:
 *        @code {.bash}
 *            g++ -I /usr/lib/boost_1_79_0 src/main.cpp -o main -lrt
 *        @endcode
 *        @code {.powershell}
 *            g++ -I C:\Users\vagrant\boost_1_79_0\ .\src\main.cpp -o main
 *        @endcode
 * 
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */

#define REGION_SIZE (100)

int main(int argc, char *argv[])
{
   using namespace boost::interprocess;

   printf("BRET argc %d\n", argc);
   printf("BRET start of PID %d\n", ::getpid());

   if(argc == 1){  //Parent process
      //Remove shared memory on construction and destruction
      struct shm_remove
      {
         shm_remove() { shared_memory_object::remove("MySharedMemory"); }
         ~shm_remove(){ shared_memory_object::remove("MySharedMemory"); }
      } remover;

      //Create a shared memory object.
      shared_memory_object shm (create_only, "MySharedMemory", read_write);

      //Set size
      shm.truncate(REGION_SIZE);

      //Map the whole shared memory in this process
      mapped_region region(shm, read_write);

      //Write all the memory to 1      
      char *mem = static_cast<char*>(region.get_address());
      for (std::size_t i = 0; i < region.get_size(); i++)
        *mem++ = static_cast<char>(i);
      
      printf("PARENT region starts at %p, size %ld\n", region.get_address(), region.get_size());

      //Launch child process
      std::string s(argv[0]); s += " child ";
      if(0 != std::system(s.c_str()))
         return 1;
   }
   else{
      //Open already created shared memory object.
      shared_memory_object shm (open_only, "MySharedMemory", read_only);

      //Map the whole shared memory in this process
      mapped_region region(shm, read_only);

      //Check that memory was initialized to 1
      char *mem = static_cast<char*>(region.get_address());

      printf("CHILD  region starts at %p, size %ld\n", region.get_address(), region.get_size());

      for(std::size_t i = 0; i < region.get_size(); ++i)
         printf("%d, ", *mem++);
      printf("\n");
   }

   printf("BRET end of PID %d\n", ::getpid());
   return 0;
}