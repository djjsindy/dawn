memcached协议队列
====

features:

   buddy memory allocation
   
   async persistence queue data
   
   config module
   
测试数据：对比kestrel

      单线程写入360字节数据10000次
      dawn用时:2160ms
      kestrel用时:3860ms
   
