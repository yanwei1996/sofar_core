/** 
 * @file          sdk_fs.h
 * @brief         文件操作有关的函数功能说明
 * @author        qinmingsheng
 * @version       V0.0.1     初始版本
 * @date          2022/12/20 11:12:30
 * @lastAuthor    qinmingsheng
 * @copyright     Copyright (c) 2022 by SofarSolar, All Rights Reserved. 
 */
 #ifndef __SDK_FS_H__
 #define __SDK_FS_H__
 #include <stdint.h>
 #include <stdio.h>
  
  
 #ifdef  __cplusplus
 extern "C"
 {
 #endif

#define PATH_NAME_LEN_LIMIT     128
  
 typedef struct
 {
     FILE file;
 }fs_t;
    
typedef enum
 {
     FS_READ = (1 << 0),            //读操作    补充, PS:文件存在，按rt打开
     FS_WRITE = (1 << 1),           //写操作    补充, PS:文件存在，按wt打开
     FS_OPEN_EXISTING = (1 << 2),   //打开文件，如果文件不存在返回失败             PS:文件存在，按rt+(|FS_WRITE时)/rt打开；文件不存在，返回NULL； 
     FS_CREATE_NEW = (1 << 3),      //创建新文件，如果文件已存在则返回失败         PS:文件存在，返回NULL；文件不存在，按wt+(|FS_READ时)/wt 打开
     FS_CREATE_ALWAYS = (1 << 4),   //创建新文件，如果文件已存在则清空该文件内容   PS:按wt+(|FS_READ时)/wt 打开
     FS_OPEN_ALWAYS = (1 << 5),     //打开文件，如果文件不存在则创建该文件         PS:文件存在，按rt+(|FS_WRITE时)/rt打开；文件不存在，按wt+(|FS_READ时)/wt 打开     
     FS_OPEN_APPEND = (1 << 6),     //打开文件，和FS_OPEN_ALWAYS一样，但是每次写头跳转到文件尾部    PS:按at+(|FS_READ时)/at打开
 }fs_open_mode_e;

typedef enum
{
    F_OK = (0 << 0),    /* Test for existence. */
    X_OK = (1 << 0),    /* Test for execute permission. */    
    W_OK = (1 << 1),    /* Test for write permission. */ 
    R_OK = (1 << 2),    /* Test for read permission. */    
}fs_test_mode_e;

typedef enum
{
    S_IRWXU = 00700,    // 文件所有者具有读、写、执行权限   （八进制数）
    S_IRUSR = 00400,    // 文件所有者具有读权限
    S_IWUSR = 00200,    // 文件所有者具有写权限
    S_IXUSR = 00100,    // 文件所有者具有执行权限
 
    S_IRWXG = 00070,    // 用户组具有读、写、执行权限
    S_IRGRP = 00040,    // 用户组具有读权限
    S_IWGRP = 00020,    // 用户组具有写权限
    S_IXGRP = 00010,    // 用户组具有执行权限
 
    S_IRWXO = 00007,    // 其他用户具有读、写、执行权限
    S_IROTH = 00004,    // 其他用户具有读权限
    S_IWOTH = 00002,    // 其他用户具有写权限
    S_IXOTH = 00001,    // 其他用户具有执行权限
}fs_author_mode_e;
  
 /** 
  * @brief        对文件系统初始化进行相关操作，如：挂载文件系统。 
  * @return       [int32_t] 执行结果
  * @retval       >=0成功
  * @retval       <0失败
  */ 
 int32_t sdk_fs_init(void);

 /** 
  * @brief        对指定路经的文件按照设定的模式进行打开操作
  * @param        [in] path 文件所在的路经及文件名，可包含盘符  (路径名称不超过PATH_NAME_LEN_LIMIT个字符)
  * @param        [in] mode 打开模式，不同模式可进行位或操作,具体见fs_open_mode_e
  * @return       [fs_t] 执行结果
  * @retval       >0 成功，返回指向该流的文件指针
  * @retval       <0 失败，
  */ 
 fs_t *sdk_fs_open(const int8_t *path, uint16_t mode);

 /** 
  * @brief        关闭文件指针把指向的文件
  * @param        [in] p_fs 需要关闭文件的文件指针
  * @return       [int32_t] 执行结果
  * @retval       =0 关闭成功
  * @retval       < 关闭失败，返回错误代码
  */
 int32_t sdk_fs_close(fs_t *p_fs);

 /** 
  * @brief        向文件指针所指向的流中读取指定长度的数据
  * @param        [in] p_fs 已打开的文件指针
  * @param        [out] buff 指向欲存放读取进来的数据空间
  * @param        [in] len 欲读取数据长度
  * @return       [int32_t] 执行结果
  * @retval       >=0 返回实际读取到的数据长度，单位字节
  * @retval       <0 读取失败
  */  
 int32_t sdk_fs_read(fs_t *p_fs, void *buff, uint32_t len);

 /** 
  * @brief        将数据写入到文件指针所指向的数据流中
  * @param        [in] p_fs 已打开的文件指针
  * @param        [in] buff 指向欲写入的数据地址
  * @param        [in] len 欲写入数据的长度
  * @return       [int32_t]执行结果
  * @retval       >=0 返回实际写入的数据长度，单位字节
  * @retval       <0 读取失败
  */  
 int32_t sdk_fs_write(fs_t *p_fs, void *buff, uint32_t len);
  
 /** 
  * @brief        移动文件流的读写位置
  * @param        [in] p_fs 已打开的文件指针
  * @param        [in] offset 根据当前位置移到读写位置的偏移量
  * @return       [int32_t] 执行结果
  * @retval       =0 成功
  * @retval       <0 错误，值对应的是错误代码
  */  
 int32_t sdk_fs_lseek(fs_t *p_fs, uint32_t offset);
  
 /** 
  * @brief        读取文件的大小
  * @param        [in] p_fs 已打开的文件指针
  * @return       [int32_t] 执行结果
  * @retval       >0 获取成功，返回文件大小，单位字节
  * @retval       <0 获取失败
  */  
 int32_t sdk_fs_get_size(fs_t *p_fs);
  
 /** 
  * @brief        保存文件流数据到存储介质
  * @param        [in] p_fs 已打开的文件指针
  * @return       [int32_t] 执行结果
  * @retval       =0成功
  * @retval       <0失败，值对应不同的错误代码
  */  
 int32_t sdk_fs_file_sync(fs_t *p_fs);
  
 /** 
  * @brief        删除路经中指定的文件
  * @param        [in] path 文件所在的路经及文件名，可包含盘符
  * @return       [int32_t] 执行结果
  * @retval       =0 成功
  * @retval       <0 失败，值对应不同的错误代码
  */  
 int32_t sdk_fs_remove(const int8_t *path);

 /** 
  * @brief        移动目录或者修改文件名字（原文件及目标目录必须存在,否则失败）
  * @param        [in] oldpath 文件所在的旧路经及文件名，可包含盘符
  * @param        [in] newpath 新路经及文件名，可包含盘符
  * @return       [int32_t]执行结果
  * @retval       =0 成功
  * @note         <0 失败，值对应不同的错误代码
  */  
 int32_t sdk_fs_rename(const int8_t *oldpath, const int8_t *newpath);

/**
* @brief		创建目录（支持多级目录递归创建)
* @param		[in] path 创建的目录路径
* @param		[in] mode 要创建的目录权限模式(具体见fs_author_mode_e)
* @return		结果
* @retval		0 成功
* @retval		< 0 失败
*/
 int32_t sdk_fs_mkdir(const char *path, uint32_t mode);

 /**
* @brief		确定文件或者文件夹的访问权限
* @param		[in] path 文件的目录路径
* @param		[in] mode 要判断的模式(具体见fs_test_mode_e，可以是各种值的组合)
* @return		结果
* @retval		0 指定的访问权限存在
* @retval		-1 指定的访问权限不存在
* @retval		ERR_PARA 参数错误
*/
 int32_t sdk_fs_access(const int8_t *p_path, int32_t mode);


 #ifdef  __cplusplus
 }  
 #endif

 #endif
 