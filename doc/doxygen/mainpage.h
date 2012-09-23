/*
 * =============================================================================
 *
 *       Filename:  mainpage.h
 *
 *    Description:  mainpage content
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

/** @mainpage OSSC 开发者手册
*******************************************************************************
* @section OSSC介绍
* @subsection 关于OSS
* 阿里云存储服务(OpenStorageService,简称 OSS)，是阿里云对外提供的海量，安全，低成本，高可靠的云存储服务。
* 用户可以通过简单的 REST 接口，在任何时间、任何地点、任何互联网设备上进行上传和下载数据，
* 也可以使用WEB 页面对数据进行管理。同时，OSS 提供 Java、Python、PHP SDK,简化用户的编程。
* 基于 OSS，用户可以搭建出各种多媒体分享网站、网盘、个人企业数据备份等基于大规模数据的服务。
* 【摘自《OSS API 开放接口规范文档》】。
*
* @subpage OSSC_INTRO
* @subsection 我们为OSS做了什么？
* 本项目为阿里云开放存储服务（OSS）提供了一套完整易用的 C SDK，并取名为 OSSC，
* 目前 OSSC 提供了 OSS 开放接口中所描述的所有功能, 特点包括：
* -# Bucket 所有操作，如创建 Bucket、删除 Bucket、获取某个 Bucket 访问权限、设置 Bucket 访问权限、获取所有 Bucket 信息、
* 获取 Bucket 中所有 Object 的信息。
* -# Object 所有操作，创建 Object（PUT），获取 Object，删除 Object，获取 Object 元信息，拷贝 Object，一次性删除多个 Object，另外，还在此基础上\n
* 实现了从文件上传 Object、从内存缓冲区上传 Object、下载 Object 至文件、下载 Object 至内存缓冲区、多线程断点续传上传大文件。
* -# Multipart Upload 操作，初始化 Multipart Upload、上传 Part、完成 Multipart 上传、终止 Multipart Upload、查看 Multipart Upload，查看正在上传的 Part。
* -# Object Group 操作，创建 Object Group，获取 Object Group，获取 Object Group 中的 Object List 信息、获取 Object Group 元信息，删除 Object Group。
*******************************************************************************
* @section OSSC安装细节
* @subpage OSSC_INSTALL
* @subsection OSSC依赖库
* OSSC 采用 CURL 库处理 HTTP 请求，因此在编译 OSSC 之前你需要安装 CURL，CURL 源码中包含了C调用API，最新版 CURL下载地址：http://curl.haxx.se/libcurl/。
* 除此之外，OSSC 不依赖任何其他程序库。
*
* @subsubsection CURL安装
* 本节介绍如何编译 CURL
*
* -# 下载 CURL，http://curl.haxx.se/download.html
* -# 安装 CURL，在Unix/Linux按照如下步骤，
* @code
* $ ./configure
* $ make
* $ make test (optional)
* $ make install(需root用户权限)
* @endcode
* 你也可以参考 CURL 的官方安装文档，http://curl.haxx.se/docs/install.html
* -# 完成依赖库安装后执行 ldconfig（需root用户权限）
*
* @subsection OSSC编译步骤
* 本节介绍如何编译OSSC。
*
* OSSC采用 CMake 管理构建过程，应该先安装CMake，
* -# 安装CMake。
* -# 创建 build 目录，进入到该目录，执行 cmake ../.
* -# 编译和安装 make && make install
*
* OSSC默认安装在 /usr/local目录下，当然你可以在 cmake 中设置。
*
* @subsection 在你的程序中使用OSSC
* OSSC以程序库的形式提供给上层开发者使用，因此如果你想基于OSSC开发上层应用，必须链接OSSC程序库，OSSC大部分都集中到了 osscore 中，\n
* 只是支持多线程调用模式的API单独放在 ossextra库中（由于时间有限，目前多线程只支持 pthread 线程库，后期会考虑在 Windows 下也支持多线程）\n
* 以下是你的程序需要连接OSSC，链接参数为：-L/path-to-your-ossc-installation -losscore.
*
* 另外需要注意的是，OSSC支持多线程断点续传模式下上传和下载文件，如果你想体验该功能，你需要链接如下库：
* -L/path-to-your-ossc-installation -lossextra.
*******************************************************************************
* @section OSSC编码规范
* 一个优秀的项目必须遵循良好的编码规范，良好的编码风格可以促进团队协作，减少 BUG 产生几率，有助于开发后的代码审查，降低后期维护成本。
*
* OSSC虽然只是我们在业余时间完成的小项目，但是也注重的编码风格的一致性，我们强烈建议那些希望基于OSSC做二次开发的编码人员阅读本规范，
* 相信本规范能够让你快速熟悉 OSSC 的整体结构和OSSC API 的使用方法。
*
* @subpage OSSC_CODING_STYLE
*******************************************************************************
* @section OSSC实现原理
*
* @subsection 概括
* OSSC（OSS-C-SDK）完全采用C语言开发，并实现了类似面向对象的调用方式，即“对象（struct 结构）”的“成员函数（函数指针）“采用函数指针形式实现，
* 我们尽量和OSS-JAVA的使用方式切合，每个对象都有相关的头文件和实现文件，并存放在modules目录下，其中与oss功能相关的所有的文件名均以oss_为前缀，\n
* 每个文件实现了一个对象（即模块），另外，为了方便开发人员记忆，OSSC中的所有结构和函数我们采用如下命名；
*
* -# 类名（本文指代struct 结构）以 “oss_” 为前缀，并以 “_t” 为后缀，名字中间的单词用来标识该结构的用途，单词以下划线“_”连接，比如：包含终止一个Multipart上传事件的请求参数被命名为 \n
* oss_abort_multipart_upload_request_t，或者包含访问阿里云开放存储服务的入口“类”被命名为 oss_client_t，其他以此类推。
* -# 与各个类（struct 结构指针，以下均以“类”指代）对应的函数均以函数指针的形式调用（除了oss_client_t），比如oss_abort_multipart_upload_request_t中设置 Bucket 名称可以按照如下方式调用，
* @code
* request->set_bucket_name(request，"bucket-name-001");
* @endcode
* -# 所有类对外均提供初始化函数和析构函数，其它与该类相关的操作均通过该类的函数指针调用，其中初始化函数命名规则为：该类的(BARE)类名（即类名去掉“oss_”前缀和“_t”后缀）+initialize，
* 析构函数为该类的(BARE)类名（即类名去掉“oss_”前缀和“_t”后缀）+finalize。
* -# 访问阿里云开放存储服务（Open Storage Service， OSS）的入口“类”是 oss_client_t，与此对应的所有函数均以client_前缀开头，并且第一个参数都是指向client结构的指针。
*
* 比如上传一个对象，你需要定义标识上传对象的请求结构（oss_put_object_request_t *request）和\n
* 上传对象之后返回信息的结构(oss_put_object_result_t *result)，然后将request作为参数传递给上传对象函数中（client_put_object_from_file(）\n
* 或者client_put_object()中），最后返回值会保存在result指针所指向的结构中。
*
* 下面的代码概括了使用 OSSC 的步骤(单线程上传文件示例)：
* @code
* const char *access_id = "ACSGmv9fkV1TDO9L";// Access ID
* const char *access_key = "BedoWbsje2"; // Access key
* onst char *bucket_name = "bucketname001"; //Bucket 名称
* const char *key = "put-test.data"; // Key, Object 名称
*
* FILE *file = fopen("proactor.pdf", "r"); //打开需要上传的文件
* unsigned short retcode = 0; //设置返回码，关于返回码的定义将在后文中详细介绍
* oss_client_t *client = client_initialize(access_id, access_key); // 初始化 oss_client_t *clien
*
* oss_object_metadata_t *metadata = object_metadata_initialize(); // 初始化 oss_object_metadata_t *metadata
* metadata->set_content_length(metadata, file_len); // 设置 metadata 元信息
* metadata->set_content_type(metadata, "application/octet-stream"); // 设置 metadata 元信息
*
* client_put_object_from_file(client, bucket_name, key, file, metadata, &retcode);// 核心操作：调用上传对象API
* client_finalize(client); //使用以后程序退出时需要释放 oss_client_t *client 分配的空间
*
* @endcode
*
* OSSC详细的实现原理请参考：\subpage OSSC_INTERNAL
*******************************************************************************
* @section OSSC高级模块Extra库
* OSSC 高级模块中包含了多线程上传大文件的 API，并支持断点续传，由于时间和精力有限，我们目前并没有实现 Windows 平台的多线程上传下载功能，希望今后会有其他开发者实现
* 这一功能。
*
* OSSC 采用了POSIX多线程标准库 pthread,理论上只要你的操作系统支持 pthread都可以使用 OSSC 的 extra 库中的 API。

* @subpage OSSC_EXTRA
*******************************************************************************
* @section API使用示例
*
* @subpage OSSC_API_EXAMPLE
*******************************************************************************
* @section 关于OSSC授权
* OSSC 采用 LGPL（GNU Lesser General Public License：GNU 宽通用公共许可证）授权形式发布，有关 LGPL 你可以查阅 GNU 官方文档：
* http://www.gnu.org/licenses/lgpl.html
*******************************************************************************
* @section 关于作者
* 傅海平：中国科学院计算技术研究所网络数据中心学生(haipingf@gmail.com)\n
* 王  维：中国科院学计算技术研究所网络数据中心学生(wangwei881116@gmail.com)
*/


/////////////////////////////////////////////////////////////////////////////////////////////////////
/** @page OSSC_INTRO OSSC介绍
 * @section OSSC介绍
 * @subsection OSS 简介
* 阿里云存储服务(OpenStorageService,简称 OSS)，是阿里云对外提供的海量，安全，低成本，高可靠的云存储服务。
* 用户可以通过简单的 REST 接口，在任何时间、任何地点、任何互联网设备上进行上传和下载数据，
* 也可以使用WEB 页面对数据进行管理。同时，OSS 提供 Java、Python、PHP SDK,简化用户的编程。
* 基于 OSS，用户可以搭建出各种多媒体分享网站、网盘、个人企业数据备份等基于大规模数据的服务。
* 【摘自《OSS API 开放接口规范文档》】。
*
* @subsection OSSC概述
* 本项目为阿里云开放存储服务（OSS）提供了一套完整易用的 C SDK，并取名为 OSSC，
* 目前 OSSC 提供了 OSS 开放接口中所描述的所有功能, 特点包括：
* - Bucket 所有操作，如创建 Bucket、删除 Bucket、获取某个 Bucket 访问权限、设置 Bucket 访问权限、获取所有 Bucket 信息、
* 获取 Bucket 中所有 Object 的信息。
*
* - Object 所有操作，创建 Object（PUT），获取 Object，删除 Object，获取 Object 元信息，拷贝 Object，一次性删除多个 Object，另外，还在此基础上\n
* 实现了从文件上传 Object、从内存缓冲区上传 Object、下载 Object 至文件、下载 Object 至内存缓冲区、多线程断点续传上传大文件，断点续传下载文件。
*
* - Multipart Upload 操作，初始化 Multipart Upload、上传 Part、完成 Multipart 上传、终止 Multipart Upload、查看 Multipart Upload，查看正在上传的 Part。
*
* - Object Group 操作，创建 Object Group，获取 Object Group，获取 Object Group 中的 Object List 信息、获取 Object Group 元信息，删除 Object Group。
*
* @subsection 我们的夙愿
* 我们是 OSSC（OSS C SDK） 的开发者，OSSC 为阿里云开放存储服务（OSS）提供了一套完整易用的 C SDK，目前 OSSC 相比 OSS JAVA可能还存在一些不足，程序稳定性有待检验，
* 但是在功能上完全实现了 OSS 开放接口，所以就功能而言，OSSC 已经达到在生产环境中使用的级别。
*
* 但是由于时间仓促，加上我们自身能力的有限，OSSC 目前不是十分完美，我们将它开源贡献出来，希望今后更多的人参与到 OSSC 项目上来，提高 OSSC 的稳定性，希望借此促进阿里云开放存储服务OSS的发展，同时我们也为开源贡献力量。
*
*/

/** @page OSSC_INSTALL OSSC安装步骤
* @section OSSC安装步骤
* @subsection OSSC依赖库
* OSSC 采用 CURL 库处理 HTTP 请求，因此在编译 OSSC 之前你需要安装 CURL，CURL 源码中包含了C调用API，最新版 CURL下载地址：http://curl.haxx.se/libcurl/。
* 除此之外，OSSC 不依赖任何其他程序库。
*
* @subsubsection CURL安装
* 本节介绍如何编译 CURL
*
* -# 下载 CURL，http://curl.haxx.se/download.html
* -# 安装 CURL，在Unix/Linux按照如下步骤，
* @code
* $ ./configure
* $ make
* $ make test (optional)
* $ make install(需root用户权限)
* @endcode
* 你也可以参考 CURL 的官方安装文档，http://curl.haxx.se/docs/install.html
* -# 完成依赖库安装后执行 ldconfig（需root用户权限）
*
* @subsection OSSC编译步骤
* 本节介绍如何编译OSSC。
*
* OSSC采用 CMake 管理构建过程，应该先安装CMake，
* -# 安装CMake。
* -# 创建 build 目录，进入到该目录，执行 cmake ../.
* -# 编译和安装 make && make install
*
* OSSC默认安装在 /usr/local目录下，当然你可以在 cmake 中设置。
*
* @subsection 在你的程序中使用OSSC
* OSSC以程序库的形式提供给上层开发者使用，因此如果你想基于OSSC开发上层应用，必须链接OSSC程序库，OSSC大部分都集中到了 osscore 中，\n
* 只是支持多线程调用模式的API单独放在 ossextra库中（由于时间有限，目前多线程只支持 pthread 线程库，后期会考虑在 Windows 下也支持多线程）\n
* 以下是你的程序需要连接OSSC，链接参数为：-L/path-to-your-ossc-installation -losscore.
*
* 另外需要注意的是，OSSC支持多线程断点续传模式下上传和下载文件，如果你想体验该功能，你需要链接如下库：
* -L/path-to-your-ossc-installation -lossextra.
 */

/** @page OSSC_CODING_STYLE OSSC编码规范
* @section 编码规范的重要性
* 如前文所述，一个优秀的项目必须遵循良好的编码规范，良好的编码风格可以促进团队协作，减少 BUG 产生几率，有助于开发后的代码审查，降低后期维护成本。
* @section OSSC编码规范概述
* - OSSC 所有函数名和类名（即结构名，下同）均已小写字符书写，全局枚举量，常量和宏定义在include/ossc/oss_constants.h中。
*
* - 类名以 “oss_” 为前缀，并以 “_t” 为后缀，名字中间的单词（均为小写）用来标识该结构的用途，单词以下划线“_”连接，比如：包含终止一个Multipart上传事件的请求参数被命名为
* oss_abort_multipart_upload_request_t，或者包含访问阿里云开放存储服务的入口“类”被命名为 oss_client_t，其他以此类推。
*
* - 与各个类对应的函数均以函数指针的形式调用（除了oss_client_t, oss_client_t类相关的函数均以 "client_"为前缀，第一个参数是 oss_client_t 指针，比如client_create_bucket()），比如oss_abort_multipart_upload_request_t中设置 Bucket 名称可以按照如下方式调用，
* @code
* request->set_bucket_name(request，"bucket-name-001");
* @endcode
* @note 调用规则：类（即结构，如前文所述）的指针调用其内部的函数指针所指向的函数，该函数的第一个参数是其自身结构的指针，第二个参数开始就是该函数真正的参数，注意，这与 C++ 或 Java 的调用习惯相似，只是第一个参数固定为指向自身结构的指针。
*
* - 所有类对外均提供初始化函数和析构函数，其它与该类相关的操作均通过该类的函数指针调用，其中初始化函数命名规则为：该类的(BARE)类名（即类名去掉“oss_”前缀和“_t”后缀）+initialize，
* 析构函数为该类的(BARE)类名（即类名去掉“oss_”前缀和“_t”后缀）+finalize。
*
* - 访问阿里云开放存储服务（Open Storage Service， OSS）的入口“类”是 oss_client_t，与此对应的所有函数均以client_前缀开头，并且第一个参数都是指向client结构的指针。
* @section 源码目录介绍
* OSSC 的源码目录如下：
* @code
* .
* ├── AUTHORS
* ├── build
* ├── CMakeLists.txt
* ├── config.h.cmake.in
* ├── doc
* ├── include
* ├── LICENSE.txt
* ├── README.txt
* ├── src
* ├── example
* @endcode
*
* 其中
* - AUTHORS文件是作者（包括贡献者）的基本信息；
*
* - build 目录是构建目录，建议在该目录下构建 OSSC，实际上由于 OSSC 采用的是 CMake OUT-OF-SOURCE 构建方式，所以只能在该目录下编译 OSSC；
*
* - CMakeList.txt 是项目的顶级 CMake 脚本；
*
* - config.h.cmake.in 是 CMake 待检测的编译选项；
*
* - doc 目录包含了 OSSC 的所有文档；
*
* - include目录是包含了OSSC 所需的头文件；
*
* - LICENSE.txt 是OSSC的项目采用的开源条款；
*
* - README.txt是说明文档；
*
* - src是OSSC的全部源码；
*
* - example目录包含了一些 OSSC 的实际使用例子。
*
* @section 命名规则
*
* OSSC 完全采用C开发，遵循一套完整的命名规则，具体如下：
* @subsection 文件命名规则
* OSSC 实现了面向对象的调用方式，类的所有函数均以函数指针的方式实现。所有文件名均以 oss_ 开头，其中各个类(结构)的头文件主要存放在 include/ossc/modules目录下面，
* 对应的实现文件则存放在 src/modules 目录中，另外每个类的头文件和实现文件命名规则为 "oss_" + 类名小写（单词间采用下划线 “_” 连接），
* 比如 client 类（OSS 服务入口类）的实现文件是 src/moduels/oss_client.c，而对应的头文件则是 include/ossc/modules/oss_client.h。
*
* @subsection 类名，函数名，变量名，常量名，宏等命名规则
*
* - 类名以 “oss_” 为前缀，并以 “_t” 为后缀（内部定义为 struct oss_class_name_s），名字中间的单词（均为小写）用来标识该结构的用途，单词以下划线“_”连接，如 client 类命名为 oss_client_t。
*
* - OSSC 函数分为内部使用函数和提供给外部接口函数两类，其中内部使用的函数使用下划线“_”开头，另外函数名也均以小写书写，单词间以下划线“_”连接，
* OSSC 提供外部使用的函数也分为两小类，一种是 helper 函数，即辅助函数，此类函数也以 "oss_" 开头，后面跟实际函数名，一般而言函数名尽量体现函数功能，如计算 md5 的函数 oss_compute_md5_digest()，
* 或者获取文件大小 oss_get_file_size()；另外一种是 core 函数，即 client类 （oss_client_t）的核心函数，此类函数均已 “cilent_”开头，其后名称也尽量体现该函数的功能，
* 比如下载 Object 至内存的函数是 client_get_object_to_buffer()，创建 Bucket 的函数名为 client_create_bucket();
*
* - 变量名均采用小写字母命名，单词间采用下划线连接“_”，可以合理缩写，但是应以准确易读为标准，对于无意义的循环变量，可以使用诸如 i, j, k类似的单个字母，但是并不推荐该方法。
*
* - 常量名（const）以“k”开头，其余字母或单词均用大写，如 kMYVAR，kPAGESIZE等。
*
* - 宏名均采用大写字母，一般以“OSS”开头，单词间以下划线“_“连接，如 OSS_HTTP_GET, OSS_CONTENT_TYPE。
*
* @subsection 其他命名规则
* 其他命名规则比较宽泛，OSSC 不作严格要求。
*
* @section 排版
* 以下内容部分参考:
* @sa https://innosoc.googlecode.com/files/%E7%BC%96%E7%A8%8B%E8%A7%84%E8%8C%83.pdf
* @subsection 缩进与对齐
* 缩进代码统一采用 4 格缩进,4 个空格或 Tab 字符,若采用 Tab 字符,应调整编辑器将 Tab 宽度设置为 4。另外，不要在代码中混合使用 Tab 和空格来进行缩进。
* @subsection 放置花括号
* 对于 if, for, while 等关键字，起始花括号放在该行的末尾，而对于函数的起始花括号则新起一行放置，如下面例子所示：
* @code
* static inline void
* __bucket_set_name(
* 		oss_bucket_t *bucket,
* 		const char *name,
* 		size_t name_len)
* {
* 	if (bucket->name != NULL) {
* 		free(bucket->name);
* 		bucket->name = NULL;
* 	}
* 	bucket->name = (char *)malloc(sizeof(char) * name_len + 1);
* 	strncpy(bucket->name, name, name_len);
* 	(bucket->name)[name_len] = '\0';
* }
* @endcode
* @subsection 空格
* - 双目运算符两侧,需要留有空格。如 “=”, “+=”,“>=”, “<=”, “+”, “&&”, “<<”, “^”等。
*
* - if, for, while, switch 等关键字之后,与紧接着的左括号‘(’之间,要留有一个空格。
*
* - 左花括号“{”之前，右括号“)”与左花括号“{”之间要留一空格。
*
* - 函数参数之间逗号‘,’分隔后面要留有空格。
*
* - for 语句分号‘;’分隔后面要留有空格。
*
* @subsection 空行
* - 函数定义的前后,均需要适当的空行。
*
* - 在函数体内,在一组变量定义完之后,需要适当空行。
*
* - 一段逻辑性相关的代码前后需要加空行。
*
* @subsection 代码行
* - 一行代码宽度应不超过 80 个字符,对于较长的语句,应该分行书写,增加代码的可读性。
*
* @section 注释
* OSSC 采用 Doxygen 进行文档维护，具体注释风格与 JAVADOC 一致，参见 Doxygen 官方网站。@sa http://www.stack.nl/~dimitri/doxygen/docblocks.html
*
* 另外，OSSC 头文件和实现文件开头均加上了如下语句：
* @code
* /\*
*  * =============================================================================
*  *
*  *       Filename:  oss_part_listing.h
*  *
*  *    Description:  oss_part_listing.
*  *
*  *        Created:  09/04/2012 08:57:48 PM
*  *
*  *        Company:  ICT ( Institute Of Computing Technology, CAS )
*  *
*  *
*  * =============================================================================
*  *\/
* @endcode
*
*/

/** @page OSSC_INTERNAL OSSC实现原理
 *
 * @section 概述
 * OSSC（OSS-C-SDK）完全采用C语言开发，所有代码（头文件 + 实现文件 + 测试文件）合计约 2.2 万行，我们在 OSSC 中实现了类似面向对象的调用方式，\n
 * 因此熟悉 OSS Java调用方式的开发者可以在很短的时间内掌握 OSS C SDK的使用。
 * @section 代码目录结构
 * @code
 * .
 * ├── AUTHORS
 * ├── CMakeLists.txt
 * ├── config.h.cmake.in
 * ├── doc
 * │   └── doxygen
 * ├── include
 * │   ├── ossc
 * │   └── ossc-config.h
 * ├── LICENSE.txt
 * ├── README.txt
 * ├── src
 * │   ├── CMakeLists.txt
 * │   ├── core
 * │   ├── extra
 * │   ├── lib
 * │   ├── modules
 * │   └── util
 * ├── test
 *
 * @endcode
 *
 * OSSC 采用 CMake 管理项目构建过程，源码主要存放在 src 中，头文件在存放在 include 中，其中 include 目录结构如下：
 * @code
 * ├── ossc
 * │   ├── client.h
 * │   ├── modules
 * │   ├── oss_constants.h
 * │   ├── oss_curl_callback.h
 * │   ├── oss_extra.h
 * │   ├── oss_helper.h
 * │   └── util
 * └── ossc-config.h
 * @endcode
 * 如编码规范一节所述，modules目录主要存放了各个类（即 C 语言中结构 struct）的头文件，你不应该直接包含include这些头文件，因为modules下的头文件开始都包含了以下语句（各个头文件可能不同）：
 * @code
 * #ifndef _OSS_PART_LISTING_H
 * # error Never include <ossc/modules/oss_part_listing.h> directly,
 * # error use <ossc/client.h> instead.
 * #endif
 * @endcode
 *
 * 如果你想使用 OSSC 的 API，你可以只包含 ossc/client.h文件即可，为了保险起见，你应该尽量包含 ossc/oss_helper.h 或 ossc/oss_extra.h（依赖pthread，仅在Unix/Linux下有效），ossc/util/oss_common.h 如下代码所示：
 *
 * @code
 * #include <ossc/client.h>
 * #include <ossc/oss_helper.h>
 * #include <ossc/oss_extra.h> // 多线程断点上传大文件功能，由于依赖pthread线程库，所以目前仅在Unix/Linux中可以使用。
 * #include <ossc/util/oss_common.h>
 * @endcode
 *
 *@attention
 * 其中 ossc/oss_extra.h 里面包含了多线程断点上传大文件功能，由于依赖pthread线程库，所以目前仅在Unix/Linux中可以使用。
 */


/** @page OSSC_EXTRA OSSC高级模块Extra库
* @section OSSC高级模块Extra简介
* OSSC 高级模块中包含了多线程上传大文件的 API，并支持断点续传，由于时间和精力有限，我们目前并没有实现 Windows 平台的多线程上传下载功能，希望今后会有其他开发者实现
* 这一功能。
*
* OSSC 采用了POSIX多线程标准库 pthread,理论上只要你的操作系统支持 pthread都可以使用 OSSC 的 extra 库中的 API。
*
* @section OSSC_EXTRA_API_INTRO OSSC Extra API 介绍
* OSSC Extra API 提供了有关多线程上传的 API，该 API 支持断点续传
* @code
* extern void
* client_extra_put_object(oss_client_t *client,
*	const char *bucket_name,
*	const char *key,
*	const char *local_file,
*	unsigned short *retcode);
* @endcode
* @section OSSC_EXTRA_API_INTERNAL OSSC Extra API 实现原理
* @section OSSC_EXTRA_API_USAGE OSSC Extra API使用
*/


/** @page OSSC_API_EXAMPLE OSSC API 使用示例
* @tableofcontents
* @section Bucket操作
* @subsection 创建Bucket
* @code
* #include <ossc/client.h>
* #include <stdio.h>
* 
* static const char *access_id = "ACSfLOiddaOzejOP";   //设置用户帐号
* static const char *access_key = "MUltNpuYqE";  //设置用户密码
* static const char *endpoint = "storage.aliyun.com";    //设置hostname
* 
* //为用户创建一个bucket
* int main()
* {
* 	unsigned short retcode;			//保存服务器http返回码的解析结果;
* 	const char *retinfo;            //保存通过retcode获得的错误信息
* 
* 	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
* 	const char *bucket_name = "bucket_example";       //设置bucket_name
* 
* 	client_create_bucket(client, bucket_name, &retcode);
* 	printf("create_bucket result:\n\n");
* 	if(retcode == OK) {
* 		printf("create bucket successfully.\n");
* 	} else {
* 		retinfo = oss_get_error_message_from_retcode(retcode);
* 		printf("error = %s\n", retinfo);
* 	}
* 
* 	client_finalize(client);
* 	return 0;
* }
* @endcode
* @subsection 获取Bucket的ACL信息
* @code
* 
* #include <ossc/client.h>
* #include <stdio.h>
* 
* static const char *access_id = "ACSfLOiddaOzejOP";   //设置用户帐号
* static const char *access_key = "MUltNpuYqE";  //设置用户密码
* static const char *endpoint = "storage.aliyun.com";    //设置hostname
* 
* //获得指定bucket的访问权限
* int main()
* {
* 	unsigned short retcode;			//保存服务器http返回码的解析结果
* 	const char *retinfo;            //保存通过retcode获得的错误信息
* 
* 	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
* 	const char *bucket_name = "xbucket_example";    //设置bucket_name
* 
* 	oss_access_control_list_t *acl = 
* 		client_get_bucket_acl(client, bucket_name, &retcode);
* 	oss_owner_t *owner = NULL;
* 	printf("get_bucket_acl result:\n\n");
* 	if(retcode == 0) {
* 		printf("get_bucket_acl successfully.\n");
* 		owner = acl->get_owner(acl);
* 		printf("grant = %s\nid = %s\ndisplay_name = %s\n", 
* 				acl->grant,
* 				owner->id,
* 				owner->display_name);
* 	} else {
* 		retinfo = oss_get_error_message_from_retcode(retcode);
* 		printf("error = %s\n", retinfo);
* 	}
* 	if(owner != NULL) {
* 		owner_finalize(owner);
* 	}
* 	if(acl != NULL) {
* 		access_control_list_finalize(acl);
* 	}
* 
* 	client_finalize(client);
* 	return 0;
* }
* @endcode
* @subsection 设置Bucket的ACL
* @code
* #include <ossc/client.h>
* #include <stdio.h>
* 
* static const char *access_id = "";   //设置用户帐号
* static const char *access_key = "";  //设置用户密码
* static const char *endpoint = "storage.aliyun.com";    //设置hostname
* 
* //设置指定bucket的访问权限
* int main()
* {
* 	unsigned short retcode;			//保存服务器http返回码的解析结果
* 	const char *retinfo;            //保存通过retcode获得的错误信息
* 
* 	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
* 	const char *bucket_name = "";    //设置bucket_name
* 
* 	const char *acl = "";            //设置bucket的访问权限（private，public-read，public-read-write中的一个）
* 	client_set_bucket_acl(client, bucket_name, acl, &retcode);
* 	printf("set_bucket_acl result:\n\n");
* 	if(retcode == 0) {
* 		printf("set bucket acl successfully.\n");
* 	} else {
* 		retinfo = oss_get_error_message_from_retcode(retcode);
* 		printf("error = %s\n", retinfo);
* 	}
* 
* 	client_finalize(client);
* 	return 0;
* }
* @endcode
*
* @subsection 列举Bucket下的Object
* @subsubsection 按名称返回请求者指定Bucket的所有Object
* @code
* 
* #include <ossc/client.h>
* #include <stdio.h>
* 
* static const char *access_id = "ACSfLOiddaOzejOP";   //设置用户帐号
* static const char *access_key = "MUltNpuYqE";  //设置用户密码
* static const char *endpoint = "storage.aliyun.com";    //设置hostname
* 
* //返回请求者指定Bucket下的所有Object的列表
* int main()
* {
* 	unsigned short retcode;			//保存服务器http返回码的解析结果
* 	const char *retinfo;            //保存通过retcode获得的错误信息
* 
* 	int i;
* 	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
* 	const char *bucket_name = "bucket_example";
* 
* 	oss_object_listing_t *object_listing = client_list_objects_with_bucket_name(client, bucket_name, &retcode);
* 	printf("list_objects_with_bucket_name result:\n\n");
* 	if(retcode == 0) {
* 		printf("list_objects_with_bucket_name successfully.\n");
* 		printf("bucket_name = %s\nnext_marker = %s\nprefix = %s\nmarker = %s\nmax_keys = %d\ndelimiter = %s\nis_truncated = %d\n", 
* 				object_listing->bucket_name, object_listing->next_marker, object_listing->prefix, object_listing->marker,
* 				object_listing->max_keys, object_listing->delimiter, object_listing->is_truncated);
* 		for(i = 0; i < object_listing->_counts_summaries; i++) {
* 			printf("****************************************************************\n");
* 			printf("object [%d] : \n", i);
* 			printf("type = %s\netag = %s\nkey = %s\nlast_modified = %s\nsize = %ld\nstorage_class = %s\nid = %s\ndisplay_name = %s\n",
* 					object_listing->summaries[i]->type, object_listing->summaries[i]->etag,
* 					object_listing->summaries[i]->key, object_listing->summaries[i]->last_modified,
* 					object_listing->summaries[i]->size, object_listing->summaries[i]->storage_class,
* 					object_listing->summaries[i]->owner->id, object_listing->summaries[i]->owner->display_name);
* 		}
* 	} else {
* 		retinfo = oss_get_error_message_from_retcode(retcode);
* 		printf("error = %s\n", retinfo);
* 	}
* 
* 	if(object_listing != NULL) {
* 		if(object_listing->summaries != NULL) {
* 			for(i = 0; i < object_listing->_counts_summaries; i++) {
* 				owner_finalize((object_listing->summaries)[i]->owner);
* 				object_summary_finalize((object_listing->summaries)[i]);
* 			}
* 			free(object_listing->summaries);
* 		}
* 		object_listing_finalize(object_listing);
* 	}
* 
* 
* 	client_finalize(client);
* 	return 0;
* }
* @endcode
* @subsubsection 按请求参数返回请求者指定Bucket下的所有Object列表
* @code
* #include <ossc/client.h>
* #include <stdio.h>
* 
* static const char *access_id = "ACSfLOiddaOzejOP";   //设置用户帐号
* static const char *access_key = "MUltNpuYqE";  //设置用户密码
* static const char *endpoint = "storage.aliyun.com";    //设置hostname
* 
* //返回请求者指定Bucket下的所有Object的列表
* int main()
* {
* 	unsigned short retcode;			//保存服务器http返回码的解析结果
* 	const char *retinfo;            //保存通过retcode获得的错误信息
* 
* 	int i;
* 	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
* 	// 构造 request 参数 
* 	oss_list_objects_request_t *request = list_objects_request_initialize();
* 	request->set_bucket_name(request, "bucket_example");
* 	request->set_prefix(request,"fun/");          //设置prefix
* 	request->set_delimiter(request, "/");      //设置delimiter
* 	request->set_max_keys(request, 100);      //设置max_keys
* 	request->set_marker(request, "t");         //设置marker
* 
* 
* 	oss_object_listing_t *object_listing = client_list_objects(client, request, &retcode);
* 	printf("list_objects_with_request result:\n\n");
* 	if(retcode == 0) {
* 		printf("list_objects_with_request successfully.\n");
* 		printf("bucket_name = %s\nnext_marker = %s\nprefix = %s\nmarker = %s\nmax_keys = %d\ndelimiter = %s\nis_truncated = %d\n", 
* 				object_listing->bucket_name, object_listing->next_marker, object_listing->prefix, object_listing->marker,
* 				object_listing->max_keys, object_listing->delimiter, object_listing->is_truncated);
* 		for(i = 0; i < object_listing->_counts_summaries; i++) {
* 			printf("****************************************************************\n");
* 			printf("object [%d] : \n", i);
* 			printf("type = %s\netag = %s\nkey = %s\nlast_modified = %s\nsize = %ld\nstorage_class = %s\nid = %s\ndisplay_name = %s\n",
* 					object_listing->summaries[i]->type, object_listing->summaries[i]->etag,
* 					object_listing->summaries[i]->key, object_listing->summaries[i]->last_modified,
* 					object_listing->summaries[i]->size, object_listing->summaries[i]->storage_class,
* 					object_listing->summaries[i]->owner->id, object_listing->summaries[i]->owner->display_name);
* 		}
* 		printf("=============================================================\n");
* 		for(i = 0; i < object_listing->_counts_common_prefixes; i++) {
* 			printf("common_prefix = %s\n", (object_listing->common_prefixes)[i]);
* 		}
* 	} else {
* 		retinfo = oss_get_error_message_from_retcode(retcode);
* 		printf("error = %s\n", retinfo);
* 	}
* 
* 	//释放空间 
* 	if(object_listing != NULL) {
* 		if(object_listing->summaries != NULL) {
* 			for(i = 0; i < object_listing->_counts_summaries; i++) {
* 				owner_finalize((object_listing->summaries)[i]->owner);
* 				object_summary_finalize((object_listing->summaries)[i]);
* 			}
* 			free(object_listing->summaries);
* 		}
* 		object_listing_finalize(object_listing);
* 	}
* 
* 	client_finalize(client);
* 	return 0;
* }
* 
* 
* @endcode
* 
* @subsection 获取所有的Bucket
* @code
* #include <ossc/client.h>
* #include <stdio.h>
* 
* static const char *access_id = "ACSfLOiddaOzejOP";   //设置用户帐号
* static const char *access_key = "MUltNpuYqE";  //设置用户密码
* static const char *endpoint = "storage.aliyun.com";    //设置hostname
*
* // 返回请求者拥有的所有Bucket的列表
* int main()
* {
* 	unsigned short retcode;			//保存服务器http返回码的解析结果
* 	const char *retinfo;            //保存通过retcode获得的错误信息
* 
* 	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
* 
* 	int buckets_number;             //保存buckets的数目
* 	int i;
* 	oss_owner_t *owner = NULL;
* 	oss_bucket_t **buckets = client_list_buckets(client, &buckets_number, &retcode);
* 	printf("list buckets result:\n\n");
* 	if(retcode == 0) {
* 		printf("list buckets successfully.\n");
* 		if(buckets_number != 0) {
* 			for(i = 0; i < buckets_number; i++) {
* 				printf("***********************************************************************\n");
* 				printf("bucket [%d] : \n", i);
* 				printf("name = %s\tcreate_date = %s\n", buckets[i]->get_name(buckets[i]), buckets[i]->get_create_date(buckets[i]));
* 				owner = buckets[i]->get_owner(buckets[i]);
* 				printf("id = %s\tdisplay_name = %s\n", owner->get_id(owner), owner->get_display_name(owner));
* 			}
* 		} else {
* 			printf("empty bucket list.\n");
* 		}
* 	} else {
* 		retinfo = oss_get_error_message_from_retcode(retcode);
* 		printf("error = %s\n", retinfo);
* 	}
* 
* 	// 释放空间 
* 	if(buckets != NULL) {
* 		for(i = 0; i < buckets_number; i++) {
* 			if(buckets[i] != NULL) {
* 				bucket_finalize(buckets[i]);
* 			}
* 		}
* 		free(buckets);
* 	}
* 	if(owner != NULL) {
* 		owner_finalize(owner);
* 	}
* 
* 	client_finalize(client);
* 	return 0;
* }
* 
* 
* @endcode
* @subsection 删除Bucket
* @code
* 
* #include <ossc/client.h>
* #include <stdio.h>
* 
* static const char *access_id = "ACSfLOiddaOzejOP";   //设置用户帐号
* static const char *access_key = "MUltNpuYqE";  //设置用户密码
* static const char *endpoint = "storage.aliyun.com";    //设置hostname
* 
* // 为用户删除一个bucket
* int main()
* {
* 	unsigned short retcode;			//保存服务器http返回码的解析结果;
* 	const char *retinfo;            //保存通过retcode获得的错误信息
* 
* 	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
* 	const char *bucket_name = "bucket_example";       //设置bucket_name
* 
* 	client_delete_bucket(client, bucket_name, &retcode);
* 	printf("delete_bucket result:\n\n");
* 	if(retcode == NO_CONTENT) {
* 		printf("delete bucket successfully.\n");
* 	} else {
* 		retinfo = oss_get_error_message_from_retcode(retcode);
* 		printf("error = %s\n", retinfo);
* 	}
* 
* 	client_finalize(client);
* 	return 0;
* }
* @endcode
*
* @section Object操作
* @subsection 上传Object
* @subsubsection 从文件上传Ojbect至OSS服务器
* @code
* #include <stdio.h>
* #include <stdlib.h>
* #include <ossc/client.h>
* 
* static const char *access_id  = "ACSGmv8fkV1TDO9L"; ///设置用户 Access ID 
* static const char *access_key = "BedoWbsJe2"; /// 设置用户的 Access Key 
* static const char *endpoint   = "storage.aliyun.com";    //设置 hostname
* 
* // 将本地文件上传到云服务器上 
* int main()
* {
* 	unsigned short retcode = -1;			//保存服务器http返回码的解析结果;
* 	const char *retinfo = NULL;            //保存通过retcode获得的错误信息
* 
* 	const char *bucket_name = "bucketexample";       //设置bucket_name
* 	const char *key         = "put-ojbect.data";      //设置上传key
* 	const char *local_file  = "proactor.pdf";         //设置需要上传的文件
* 
* 	FILE *fp = fopen(local_file, "r");
* 	if (fp == NULL) {
* 		fprintf(stderr, "error in opening file %s\n", local_file);
* 		return -1;
* 	}
* 
* 	size_t file_len = oss_get_file_size(fp);
* 	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
* #if 1
* 	// 初始化元信息，并设置相关属性 
* 	oss_object_metadata_t *metadata = object_metadata_initialize(); 
* 	metadata->set_content_length(metadata, file_len);
* 	metadata->set_content_type(metadata, "application/octet-stream");
* 	metadata->set_cache_control(metadata, "no-cache");
* 	metadata->set_content_encoding(metadata, "utf-8");
* 	metadata->set_content_disposition(metadata, "attachment;");
* 	metadata->set_expiration_time(metadata, "Thu, 13 Sep 2012 21:08:42 GMT");
* 	// 将本地文件上传到云服务器上 
* 	client_put_object_from_file(client, bucket_name, key, fp, metadata, &retcode);
* #else 
* 	client_put_object_from_file(client, bucket_name, key, fp, NULL, &retcode);
* #endif
* 	if (retcode == OK) {
* 		printf("Put object from file successfully.\n");
* 	} else {
* 		retinfo = oss_get_error_message_from_retcode(retcode);
* 		printf("%s\n", retinfo);
* 	}
* 
* 	client_finalize(client);
* 	fclose(fp);
* 
* 	return 0;
* }
* @endcode
* @subsubsection 从内存块上传Ojbect至OSS服务器
* @code
* #include <stdio.h>
* #include <stdlib.h>
* #include <ossc/client.h>
* 
* static const char *access_id  = "ACSGmv8fkV1TDO9L"; ///设置用户 Access ID 
* static const char *access_key = "BedoWbsJe2"; /// 设置用户的 Access Key 
* static const char *endpoint   = "storage.aliyun.com";    //设置 hostname
* 
* // 将内存中的内容上传至云服务器中 
* int main()
* {
* 	unsigned short retcode = -1;			//保存服务器http返回码的解析结果;
* 	const char *retinfo = NULL;            //保存通过retcode获得的错误信息
* 
* 	const char *bucket_name = "bucket_example";       //设置bucket_name
* 	const char *key         = "put-ojbect.data";      //设置上传key
* 	const char *local_file  = "proactor.pdf";         //设置需要上传的文件
* 	size_t retsize = 0; // 读取文件时的返回值
* 
* 	FILE *fp = fopen(local_file, "r");
* 	if (fp == NULL) {
* 		fprintf(stderr, "error in opening file %s\n", local_file);
* 		return -1;
* 	}
* 	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
* 	size_t file_len = oss_get_file_size(fp);
* 
* 	char *buffer = (char *)malloc(sizeof(char) * file_len + 1);
* 
* 	memset(buffer, '\0', file_len + 1);
* 	retsize = fread(buffer, 1, file_len, fp);
* 
* 	if (retsize < 0) {
* 		fprintf(stderr, "error in reading file %s.\n", local_file);
* 		return -1;
* 	}
* 
* 	// 初始化元信息，并设置相关属性 
* 	oss_object_metadata_t *metadata = object_metadata_initialize(); 
* 	metadata->set_content_length(metadata, file_len);
* 	metadata->set_content_type(metadata, "application/octet-stream");
* 	metadata->set_cache_control(metadata, "no-cache");
* 	metadata->set_content_encoding(metadata, "utf-8");
* 	metadata->set_content_disposition(metadata, "attachment;");
* 	metadata->set_expiration_time(metadata, "Thu, 13 Sep 2012 21:08:42 GMT");
* 
* 	// 将内存中的内容上传至云服务器中 
* 	client_put_object_from_buffer(client, bucket_name, key, buffer, metadata, &retcode);
* 	if (retcode == OK) {
* 		printf("put object from file successfully.\n");
* 	} else {
* 		retinfo = oss_get_error_message_from_retcode(retcode);
* 		printf("%s\n", retinfo);
* 	}
* 
* 	client_finalize(client);
* 	fclose(fp);
* 	return 0;
* }
* @endcode
*
* @subsection 获取Object
* @subsubsection 获取Object到内存中
* @code
* 
* #include <stdio.h>
* #include <stdlib.h>
* #include <ossc/client.h>
* 
* static const char *access_id  = "ACSGmv8fkV1TDO9L"; //设置用户 Access ID
* static const char *access_key = "BedoWbsJe2"; // 设置用户的 Access Key
* static const char *endpoint   = "storage.aliyun.com";    //设置 hostname
* 
* // 将远程服务器上的文件下载到内存中 
* int main()
* {
* 
* 	unsigned short retcode = -1;			//保存服务器http返回码的解析结果;
* 	const char *retinfo = NULL;            //保存通过retcode获得的错误信息
* 
* 	const char *bucket_name = "bucket_example";       //设置bucket_name
* 	const char *key         = "get-ojbect.data";      //设置下载 key
* 	const char *local_file  = "local-file.data";         //设置需要保存到本地的文件名
* 	size_t file_len = -1; ///< 远程文件的长度  
* 	void *buffer = NULL; // 存放文件内容的缓冲区首指针 
* 
* 	FILE *fp = fopen(local_file, "w");
* 	if (fp == NULL) {
* 		fprintf(stderr, "error in opening file %s\n", local_file);
* 		return -1;
* 	}
* 
* 	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
* 
* 	oss_get_object_request_t *request = get_object_request_initialize(bucket_name, key);
* 	// request->set_range(request, 0, 2 * 1024);
* 
* 	// 将远程服务器上的文件下载到内存中 
* 	client_get_object_to_buffer(client, request, &buffer, &file_len, &retcode);
* 
* 	if (retcode == OK) {
* 		fwrite(buffer, file_len, 1, fp);
* 		printf("Get object to file successfully.\n");
* 		printf("File length: %d\n", file_len);
* 	} else {
* 		retinfo = oss_get_error_message_from_retcode(retcode);
* 		printf("%s\n", retinfo);
* 	}
* 
* 	client_finalize(client);
* 	fclose(fp);
* 
* 	return 0;
* }
* @endcode
*
* @subsubsection 获取Object到文件中
* @code
* #include <ossc/client.h>
* 
* static const char *access_id  = "ACSGmv8fkV1TDO9L"; ///设置用户 Access ID 
* static const char *access_key = "BedoWbsJe2"; /// 设置用户的 Access Key 
* static const char *endpoint   = "storage.aliyun.com";    //设置 hostname
* 
* // 下载云服务器中的文件至本地文件 
* int main()
* {
* 
* 	unsigned short retcode = -1;			//保存服务器http返回码的解析结果;
* 	const char *retinfo = NULL;            //保存通过retcode获得的错误信息
* 
* 	const char *bucket_name = "bucket_example";       //设置bucket_name
* 	const char *key         = "get-ojbect.data";      //设置需要下载的key
* 	const char *local_file  = "local-file.data";         //设置需要下载的本地文件名，可以和key同名
* 
* 	FILE *fp = fopen(local_file, "w");
* 	if (fp == NULL) {
* 		fprintf(stderr, "error in opening file %s\n", local_file);
* 		return -1;
* 	}
* 
* 	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
* 
* 	oss_get_object_request_t *request = get_object_request_initialize(bucket_name, key);
* 	// request->set_range(request, 0, 2 * 1024 * 1024);
* 	client_get_object_to_file(client, request, fp, &retcode);
* 
* 	if (retcode == OK) {
* 		printf("Get object to file successfully.\n");
* 	} else {
* 		retinfo = oss_get_error_message_from_retcode(retcode);
* 		printf("%s\n", retinfo);
* 	}
* 
* 	client_finalize(client);
* 	fclose(fp);
* 
* 	return 0;
* 
* }
* @endcode
*
* @subsection 拷贝Object
* @subsubsection 通过指定拷贝请求来完成拷贝Object
* @code
* #include <stdio.h>
* #include <stdlib.h>
* #include <ossc/client.h>
*
* static const char *access_id  = "ACSGmv8fkV1TDO9L"; ///设置用户 Access ID
* static const char *access_key = "BedoWbsJe2"; // 设置用户的 Access Key
* static const char *endpoint   = "storage.aliyun.com";    //设置 hostname
* // Object 拷贝操作
* int main()
* {
*	unsigned short retcode = -1;			//保存服务器http返回码的解析结果;
* 	const char *retinfo = NULL;            //保存通过retcode获得的错误信息
* 	const char *source_bucket_name = "bucketname001";
* 	const char *destination_bucket_name = "bucketname002";
* 	const char *source_key = "put.png";
* 	const char *destination_key = "PUT.png";
*
* 	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
*
* 	oss_copy_object_request_t *request = copy_object_request_initialize(
* 		source_bucket_name, source_key,
* 		destination_bucket_name, destination_key);
*
* 	request->set_modified_since_constraints(request, "Wed, 12 Sep 2012 12:06:55 GMT");
*
* 	oss_copy_object_result_t *result = client_copy_object(client, request, &retcode);
*
* 	if (result != NULL) {
* 		printf("ETag: %s", result->get_etag(result));
* 		printf("LastModified: %s\n", result->get_last_modified(result));
* 	}
*
* 	if (retcode == OK) {
* 	printf("Copy object successfully.\n");
* 	} else {
* 	retinfo = oss_get_error_message_from_retcode(retcode);
* 	printf("%s\n", retinfo);
* 	}
* 	client_finalize(client);
* }
* @endcode
*
* @subsubsection 直接指定源和目的BucketName与Key拷贝Object
* @code
* #include <stdio.h>
* #include <stdlib.h>
* #include <ossc/client.h>
* 
* static const char *access_id  = "ACSGmv8fkV1TDO9L"; //设置用户 Access ID
* static const char *access_key = "BedoWbsJe2"; // 设置用户的 Access Key 
* static const char *endpoint   = "storage.aliyun.com";    //设置 hostname
* 
* // Object 拷贝操作 
* int main()
* {
* 	unsigned short retcode = -1;			//保存服务器http返回码的解析结果;
* 	const char *retinfo = NULL;            //保存通过retcode获得的错误信息
* 
* 	const char *source_bucket_name = "bucketname001";
* 	const char *destination_bucket_name = "bucketname002";
* 	const char *source_key = "put.png";
* 	const char *destination_key = "PUT.png";
* 
* 	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
* 
* 	client_copy_object_ext(client, source_bucket_name, source_key,
* 			destination_bucket_name, destination_key, NULL);
* 
* 	if (retcode == OK) {
* 		printf("Copy object successfully.\n");
* 	} else {
* 		retinfo = oss_get_error_message_from_retcode(retcode);
* 		printf("%s\n", retinfo);
* 	}
* 
* 	client_finalize(client);
* }
* @endcode
* @subsection 获取Object元信息
* @code
* #include <stdio.h>
* #include <stdlib.h>
* #include <ossc/client.h>
* 
* static const char *access_id  = "ACSGmv8fkV1TDO9L"; // 设置用户 Access ID
* static const char *access_key = "BedoWbsJe2"; // 设置用户的 Access Key
* static const char *endpoint   = "storage.aliyun.com";    //设置 hostname
* 
* // 获取Object的元信息
* int main()
* {
* 
* 	unsigned short retcode = -1;			//保存服务器http返回码的解析结果;
* 	const char *retinfo = NULL;            //保存通过retcode获得的错误信息
* 
* 	const char *bucket_name = "bucket_example";       //设置bucket_name
* 	const char *key         = "get-ojbect.data";      //设置key
* 
* 	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
* 
* 	oss_object_metadata_t *metadata =
* 		client_get_object_metadata(client, bucket_name, key, &retcode);
*
* 	if (retcode == OK) {
* 		printf("ETag: %s\n", metadata->get_etag(metadata));
* 		printf("Content-Type: %s\n", metadata->get_content_type(metadata));
* 		printf("Content-Length: %ld\n", metadata->get_content_length(metadata));
* 		printf("Last-Modified: %s\n", metadata->get_last_modified(metadata));
* 	} else {
* 		retinfo = oss_get_error_message_from_retcode(retcode);
* 		printf("%s\n", retinfo);
* 	}
* 
* 	client_finalize(client);
* }
* @endcode
*
* @subsection 删除Object
* @code
* 
* #include <stdio.h>
* #include <stdlib.h>
* #include <ossc/client.h>
* 
* static const char *access_id  = "ACSGmv8fkV1TDO9L"; //设置用户 Access ID 
* static const char *access_key = "BedoWbsJe2"; // 设置用户的 Access Key 
* static const char *endpoint   = "storage.aliyun.com";    //设置 hostname
* 
* // 删除Object
* int main()
* {
* 
* 	unsigned short retcode = -1;			//保存服务器http返回码的解析结果;
* 	const char *retinfo = NULL;            //保存通过retcode获得的错误信息
* 
* 	const char *bucket_name = "bucket_example";       //设置bucket_name
* 	const char *key         = "delete-ojbect.data";      //设置key
* 
* 	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
* 
* 	client_delete_object(client, bucket_name, key, &retcode);
* 
* 	if (retcode == OK) {
* 		printf("Delete object successfully.\n");
* 	} else {
* 		retinfo = oss_get_error_message_from_retcode(retcode);
* 		printf("%s\n", retinfo);
* 	}
* 
* 	client_finalize(client);
* }
* @endcode
*
* @subsection 一次性删除多个Object
* @code
* #include <stdio.h>
* #include <stdlib.h>
* #include <ossc/client.h>
* 
* static const char *access_id  = "ACSGmv8fkV1TDO9L"; // 设置用户 Access ID 
* static const char *access_key = "BedoWbsJe2"; // 设置用户的 Access Key 
* static const char *endpoint   = "storage.aliyun.com";    //设置 hostname
* 
* // 设置需要删除的Object 
* const char *keys[] = {
* 	"putxxx-test.pdf",
* 	"ccan.tar.bz2",
* 	"ncurseass-5.9.tar.gz",
* 	"nmon_linsdfux_14g.tar.gz"
* };
* 
* // 删除Object
* int main()
* {
* 
* 	unsigned short retcode = -1;			//保存服务器http返回码的解析结果;
* 	const char *retinfo = NULL;            //保存通过retcode获得的错误信息
* 
* 	const char *bucket_name = "bucket_example";       //设置bucket_name
* 
* 	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
* 
* 	oss_delete_multiple_object_request_t *request = 
* 		delete_multiple_object_request_initialize(bucket_name, keys, 4, false); // 同时删除 4 个对象Object
* 
* 	client_delete_multiple_object(client, request, &retcode);
* 
* 	if (retcode == OK) {
* 		printf("Delete object successfully.\n");
* 	} else {
* 		retinfo = oss_get_error_message_from_retcode(retcode);
* 		printf("%s\n", retinfo);
* 	}
* 
* 	client_finalize(client);
* }
* @endcode
* 
* @section Multipart Multipart Upload操作
*
* @subsection 初始化Multipart-Upload操作
* @code
* #include <ossc/client.h>
* 
* static const char *access_id  = "ACSGmv8fkV1TDO9L"; ///设置用户 Access ID 
* static const char *access_key = "BedoWbsJe2"; /// 设置用户的 Access Key 
* static const char *endpoint   = "storage.aliyun.com";    //设置 hostname
* 
* // 初始化Multipart Upload操作
* int main()
* {
* 
* 	unsigned short retcode = -1;			//保存服务器http返回码的解析结果;
* 	const char *retinfo = NULL;            //保存通过retcode获得的错误信息
* 
* 	const char *bucket_name = "bucket_example";       //设置bucket_name
* 	const char *key         = "multipart-upload.data";      //设置key
* 
* 
* 	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
* 
* 
* #if 1 // 设置带元信息的Multipart Upload 上传请求 
* 
* 	oss_object_metadata_t *metadata = object_metadata_initialize();
* 
* 	// 设置上传对象的元信息 
* 	metadata->set_cache_control(metadata, "no-cache");
* 	metadata->set_content_encoding(metadata, "utf-8");
* 	metadata->set_content_disposition(metadata, "attachment;");
* 	metadata->set_expiration_time(metadata, "Thu, 15 Sep 2012 21:08:42 GMT");
* 
* 	oss_initiate_multipart_upload_request_t *request = 
* 		initiate_multipart_upload_request_initialize_with_metadata(bucket_name, key, metadata);
* #else // 不带元信息的Multipart Upload上传请求 
* 	oss_initiate_multipart_upload_request_t *request = 
* 		initiate_multipart_upload_request_initialize(bucket_name, key, NULL);
* #endif
* 
* 
* 	oss_initiate_multipart_upload_result_t *result =
* 		client_initiate_multipart_upload(client, request, &retcode);
* 	if (retcode == OK) {
* 		printf("%s\n", result->get_upload_id(result));
* 	} else {
* 		retinfo = oss_get_error_message_from_retcode(retcode);
* 		printf("%s\n", retinfo);
* 	}
* 
* 	client_finalize(client);
* 
* 	return 0;
* 
* }
* @endcode
* @subsection 上传Part
* @code
* #include <ossc/client.h>
* static const char *access_id  = "ACSGmv8fkV1TDO9L"; ///设置用户 Access ID 
* static const char *access_key = "BedoWbsJe2"; /// 设置用户的 Access Key 
* static const char *endpoint   = "storage.aliyun.com";    //设置 hostname
* 
* // 上传 Part 操作，本例演示了如果使用上传Part操作来上传一个文件，
* // 适合上传大文件，但是如果想多线程断点续传，请在Unix/Linux下使用
* // OSSC 的 extra 子程序库中的 client_extra_put_object()函数。
* int main()
* {
* 
* 	unsigned short retcode = -1;			//保存服务器http返回码的解析结果;
* 	const char *retinfo = NULL;            //保存通过retcode获得的错误信息
* 
* 	const char *bucket_name = "bucket_example";       //设置bucket_name
* 	const char *key         = "multipart-upload.data";      //设置key
* 	const char *upload_id = ""; // TODO: 设置upload id，此upload id由initiate multipart upload 操作返回 
* 
* 	FILE *fp= fopen("neo4j-community.tar.gz", "r"); // 需要通过multipart upload上传的文件 
* 	size_t file_len = oss_get_file_size(fp); // 获取文件大小 
* 	
* 	char *buffer = (char *)malloc(sizeof(char) * file_len + 1);
* 	memset(buffer, '\0', file_len + 1);
* 	// 一次性读入该文件所有内容，为简单起见，不对fread返回值做错误处理 
* 	fread(buffer, 1, file_len, fp); 
* 	const int single_request_len = 8 * 1024 * 1024; // 每个Part的大小 
* 	int requests_num = file_len / single_request_len; // 需要多少次上传 
* 	int current_part_number = 0; // 当前上传的 Part Number 
* 
* 	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
* 
* 	oss_upload_part_request_t *request = upload_part_request_initialize();
* 	request->set_bucket_name(request, bucket_name);
* 	request->set_key(request, key);
* 	request->set_upload_id(request, upload_id);
* 
* 	for (current_part_number = 0; current_part_number < requests_num + 1; current_part_number++) {
* 		// 除了最后一块的其他块，最小为 5MB，此处设置单个Part为single_request_len = 8MB 
* 		if (current_part_number < requests_num) { 
* 			request->set_part_number(request, current_part_number + 1);
* 			request->set_input_stream(request,
* 					buffer + current_part_number * single_request_len, single_request_len);
* 			request->set_part_size(request, single_request_len);
* 
* 			// 上传Part 
* 			oss_upload_part_result_t *result = client_upload_part(client, request, &retcode);
* 
* 			if (retcode == OK) { // 打印返回信息，包括 Part Number 和 ETag 值 
* 				printf("PartNumber:%d, ETag:%s\n", result->get_part_number(result), result->get_etag(result));
* 			} else { // 打印出错信息 
* 				retinfo = oss_get_error_message_from_retcode(retcode);
* 				printf("%s\n", retinfo);
* 			}
* 			// 最好此时释放 result 空间 
* 			upload_part_result_finalize(result);
* 		} else { // 最后一块，可以小于5MB 
* 			request->set_part_number(request, current_part_number + 1);
* 			request->set_input_stream(request,
* 					buffer + current_part_number *single_request_len,
* 					file_len - single_request_len * current_part_number);
* 			request->set_part_size(request, file_len - single_request_len * current_part_number);
* 
* 			// 上传Part 
* 			oss_upload_part_result_t *result = client_upload_part(client, request, &retcode);
* 			
* 			if (retcode == OK) { // 打印返回信息，包括 Part Number 和 ETag 值 
* 				printf("PartNumber:%d, ETag:%s\n", result->get_part_number(result), result->get_etag(result));
* 			} else { // 打印出错信息 
* 				retinfo = oss_get_error_message_from_retcode(retcode);
* 				printf("%s\n", retinfo);
* 			}
* 			// 最好及时释放 result 空间，否则回造成内存泄漏 
* 			upload_part_result_finalize(result);
* 		}
* 	}
* 	free(buffer);
* 	upload_part_request_finalize(request);
* 	client_finalize(client);
* 	fclose(fp);
* 	return 0;
* }
* @endcode
* @subsection 完成Multipart-Upload
* @code
* #include <ossc/client.h>
* 
* static const char *access_id  = "ACSGmv8fkV1TDO9L"; ///设置用户 Access ID 
* static const char *access_key = "BedoWbsJe2"; /// 设置用户的 Access Key 
* static const char *endpoint   = "storage.aliyun.com";    //设置 hostname
* 
* 
* // TODO: 此处设置你从 Upload Part 步骤中获取的 ETag 值
* const char *etags[] = {
* 	"ETag001",
* 	"ETag002",
* 	"...",
* 	"ETag00n"
* };
* 
* // 完成一个Multipart Upload操作
* int main()
* {
* 
* 	unsigned short retcode = -1;			//保存服务器http返回码的解析结果;
* 	const char *retinfo = NULL;            //保存通过retcode获得的错误信息
* 
* 	const char *bucket_name = "bucket_example";       //设置bucket_name
* 	const char *key         = "multipart-upload.data";      //设置key
* 	const char *upload_id = ""; // TODO:此处设置你需要终止的 Upload ID 
* 
* 	size_t parts = sizeof(etags) / sizeof(char *);
* 
* 	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
* 
* 	oss_part_etag_t **part_etag = (oss_part_etag_t **)malloc(sizeof(oss_part_etag_t *) * parts);
* 	int i = 0;
* 	for (; i < 5; i++) {
* 
* 		*(part_etag + i) = part_etag_initialize(i + 1, etags[i]);
* 	}
* 
* 	oss_complete_multipart_upload_request_t *request = 
* 		complete_multipart_upload_request_initialize(bucket_name, key, upload_id, part_etag, parts);
* 
* 	// 完成一次Multipart Upload上传操作 
* 	oss_complete_multipart_upload_result_t *result = 
* 		client_complete_multipart_upload(client, request, &retcode);
* 
* 	if (retcode == OK) {
* 		printf("Complete multipart upload successfully.\n");
* 		printf("ETag: %s\n", result->get_etag(result));
* 	} else {
* 		retinfo = oss_get_error_message_from_retcode(retcode);
* 		printf("%s\n", retinfo);
* 	}
* 	complete_multipart_upload_request_finalize(request);
* 	complete_multipart_upload_result_finalize(result);
* 	client_finalize(client);
* 
* 	return 0;
* 
* }
* @endcode
* @subsection 终止一个Multipart-Upload操作
* @code
* #include <ossc/client.h>
* 
* static const char *access_id  = "ACSGmv8fkV1TDO9L"; ///设置用户 Access ID 
* static const char *access_key = "BedoWbsJe2"; /// 设置用户的 Access Key 
* static const char *endpoint   = "storage.aliyun.com";    //设置 hostname
* 
* // 终止一个Multipart Upload操作
* int main()
* {
* 
* 	unsigned short retcode = -1;			//保存服务器http返回码的解析结果;
* 	const char *retinfo = NULL;            //保存通过retcode获得的错误信息
* 
* 	const char *bucket_name = "bucket_example";       //设置bucket_name
* 	const char *key         = "multipart-upload.data";      //设置key
* 	const char *upload_id = ""; // TODO:此处设置你需要终止的 Upload ID 
* 
* 
* 	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
* 
* 	oss_abort_multipart_upload_request_t *request = 
* 		abort_multipart_upload_request_initialize(bucket_name, key, upload_id);
* 	
* 	// 终止一个Multipart Upload操作
* 	client_abort_multipart_upload(client, request, &retcode);
* 
* 	if (retcode == OK) {
* 		printf("Abort multipart upload successfully.\n");
* 	} else {
* 		retinfo = oss_get_error_message_from_retcode(retcode);
* 		printf("%s\n", retinfo);
* 	}
* 
* 	client_finalize(client);
* 	abort_multipart_upload_request_finalize(request);
* 
* 	return 0;
* 
* }
* @endcode
*
* @subsection 列出所有正在进行的Multipart-Uploads操作
* @code
* 
* #include <ossc/client.h>
* 
* static const char *access_id  = "ACSGmv8fkV1TDO9L"; ///设置用户 Access ID 
* static const char *access_key = "BedoWbsJe2"; // 设置用户的 Access Key 
* static const char *endpoint   = "storage.aliyun.com";    //设置 hostname
* 
* // 列出所有的Multipart Upload操作
* int main()
* {
* 
* 	unsigned short retcode = -1;			//保存服务器http返回码的解析结果;
* 	const char *retinfo = NULL;            //保存通过retcode获得的错误信息
* 
* 	const char *bucket_name = "bucket_example";       //设置bucket_name
* 
* 	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
* 
* 	oss_list_multipart_uploads_request_t *request = 
* 		list_multipart_uploads_request_initialize(bucket_name);
* 
* 	oss_multipart_upload_listing_t *listing = client_list_multipart_uploads(client, request, &retcode);
* 
* 	if (retcode == OK) {
* 
* 		printf("BUCKET NAME: %s\n", listing->get_bucket_name(listing));
* 
* 		size_t upload_counts = 0;
* 		int i = 0;
* 
* 		oss_multipart_upload_t **uploads = listing->get_multipart_uploads(listing, &upload_counts);
* 		for (i = 0; i < upload_counts; i++) {
* 			printf("***********************************CONTENT %d*******************************\n", i + 1);
* 			printf("KEY: %s\n", (*(uploads + i))->get_key(*(uploads + i)));
* 			printf("UPLOAD ID: %s\n", (*(uploads + i))->get_upload_id(*(uploads + i)));
* 			printf("STORAGE CLASS: %s\n", (*(uploads + i))->get_storage_class(*(uploads + i)));
* 			printf("INITIATED: %s\n", (*(uploads + i))->get_initiated(*(uploads + i)));
* 		}
* 	} else {
* 		retinfo = oss_get_error_message_from_retcode(retcode);
* 		printf("%s\n", retinfo);
* 	}
* 
* 	list_multipart_uploads_request_finalize(request);
* 	multipart_upload_listing_finalize(listing);
* 	client_finalize(client);
* 
* 	return 0;
* }
* @endcode
* @subsection 列出一个Multipart-Upload的所有已上传的parts
* @code
* #include <ossc/client.h>
* 
* static const char *access_id  = "ACSGmv8fkV1TDO9L"; ///设置用户 Access ID 
* static const char *access_key = "BedoWbsJe2"; /// 设置用户的 Access Key 
* static const char *endpoint   = "storage.aliyun.com";    //设置 hostname
* 
* // 列出一次Multipart Upload操作已经上传的所有Parts 
* int main()
* {
* 
* 	unsigned short retcode = -1;			//保存服务器http返回码的解析结果;
* 	const char *retinfo = NULL;            //保存通过retcode获得的错误信息
* 
* 	const char *bucket_name = "bucket_example";       //设置bucket_name
* 	const char *key         = "multipart-upload.data";      //设置key
* 	const char *upload_id   = ""; // TODO: 此处设置Upload ID 
* 
* 	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
* 
* 	oss_list_parts_request_t *request = 
* 		list_parts_request_initialize(bucket_name, key, upload_id);
* 
* 	oss_part_listing_t *listing = client_list_parts(client, request, &retcode);
* 
* 	if (retcode == OK) {
* 		int part_counts = 0;
* 		int i = 0;
* 		oss_part_summary_t **parts = listing->get_parts(listing, &part_counts);
* 
* 		for (i = 0; i < part_counts; i++) {
* 			printf("********************************** CONTENT %d*******************************\n", i + 1);
* 			printf("ETAG: %s\n", (*(parts + i))->get_etag(*(parts + i)));
* 			printf("LAST_MODIFIED: %s\n", (*(parts + i))->get_last_modified(*(parts + i)));
* 			printf("PARTNUMBER: %d\n", (int)(*(parts + i))->get_part_number(*(parts + i)));
* 			printf("SIZE: %d\n", (int)(*(parts + i))->get_size(*(parts + i)));
* 		}
* 	} else {
* 			retinfo = oss_get_error_message_from_retcode(retcode);
* 			printf("%s\n", retinfo);
* 	}
* 
* 	client_finalize(client);
* 	list_parts_request_finalize(request);
* 	part_listing_finalize(listing);
* 
* 	return 0;
* }
* @endcode
* 
* @section Object Group 操作
* @subsection Post Post Object Group 操作
* @code
* #include <ossc/client.h>
* #include <stdio.h>
* 
* static const char *access_id = "ACSfLOiddaOzejOP";   //设置用户帐号
* static const char *access_key = "MUltNpuYqE";  //设置用户密码
* static const char *endpoint = "storage.aliyun.com";    //设置hostname
* 
* //设置要组合的Objects的etag值 
* static const char *etags[] = {
* 	//"6F395BF16882D154CCB448806EA8C47D",
* 	//"321F048D6C898398F3168545F6CE8551",
* 	//"249697174471EEE73202E9214B037E08",
* 	//"B78E7B470FA41AA5478CFB0EDFF7B2E6",
* 	//"E0CA28D194BC790EF3C6236231D033A7"
* };
* 
* //设置要组合的Objects的name 
* static const char *partname[] = {
* 	//"a.txt",
* 	//"b.txt",
* 	//"c.txt",
* 	//"d.txt",
* 	//"e.txt"
* };
* 
* // Post Object Group 操作将根据用户提供的 Object 信息,在 OSS 服务器端创建出一个新的 Object Group。
* //用户需要将创建该 Object Group 所需的 Object Name, E-tag 以及标识该 Object 在整个 Group 中相对位置的 Part ID
* // 按照规定的 XML 格式发给 OSS, Object 的 E-tag 值可以通过list_object功能获得。
* int main()
* {
* 	unsigned short retcode;			//保存服务器http返回码的解析结果;
* 	const char *retinfo;            //保存通过retcode获得的错误信息
* 
* 	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
* 	const char *bucket_name = "bucket_example";       //设置bucket_name
* 	const char *key = "a_empty_group_file.dat";             //设置object group的名称
* 	int i;
* 	int n = sizeof(partname)/sizeof(char *);
* 	// 构造各个Object的etag，name，ID到part_item 
* 	oss_object_group_item_t **part_item = 
* 		(oss_object_group_item_t **)malloc(sizeof(oss_object_group_item_t *) * n);
* 	for (i = 0; i < n; i++) {
* 		*(part_item + i) = object_group_item_initialize(etags[i], partname[i], i + 1);
* 	}
* 	oss_post_object_group_request_t *request = 
* 		post_object_group_request_initialize(bucket_name, key, part_item, n);
* 	oss_post_object_group_result_t *post_result = client_post_object_group(client, request, &retcode);
* 
* 	printf("post_obejct_group result:\n\n");
* 	if(retcode == OK) {
* 		printf("post_object_group successfully.\n");
* 		printf("bucket_name = %s\nkey = %s\netag = %s\nsize = %u\n", 
* 				post_result->bucket_name, post_result->key,
* 				post_result->etag, post_result->size);
* 	} else {
* 		retinfo = oss_get_error_message_from_retcode(retcode);
* 		printf("error = %s\n", retinfo);
* 	}
* 
* 	// 释放空间
* 	if(part_item != NULL) {
* 		for(i = 0; i < n; i++) {
* 			if(part_item[i] != NULL) {
* 				object_group_item_finalize(part_item[i]);
* 			}
* 		}
* 		free(part_item);
* 	}
* 	if(request != NULL) {
* 		post_object_group_request_finalize(request);
* 	}
* 	if(post_result) {
* 		post_object_group_result_finalize(post_result);
* 	}
* 	client_finalize(client);
* 	return 0;
* }
* @endcode
*
* @subsection Get01 Get Object Group 操作
* @subsubsection 获取Object-Group到内存
* @code
* #include <ossc/client.h>
* #include <stdio.h>
* 
* static const char *access_id = "ACSfLOiddaOzejOP";   //设置用户帐号
* static const char *access_key = "MUltNpuYqE";  //设置用户密码
* static const char *endpoint = "storage.aliyun.com";    //设置hostname
* 
* //读取用户指定的Object group到buffer 
* int main()
* {
* 	unsigned short retcode;			//保存服务器http返回码的解析结果;
* 	const char *retinfo;            //保存通过retcode获得的错误信息
* 
* 	const char *bucket_name = "bucket_example";       //设置bucket_name
* 	const char *key = "a_empty_group_file.dat";             //设置object group的名称
* 	char *buffer;
* 	size_t buf_len;
* 
* 	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
* 	oss_get_object_group_request_t *request = 
* 		get_object_group_request_initialize(bucket_name, key);
* 	oss_object_metadata_t *result = client_get_object_group_to_buffer(client, request, (void **)&buffer, &buf_len, &retcode);
* 
* 	printf("get_obejct_group_to_buffer result:\n\n");
* 	if(retcode == OK) {
* 		printf("get_object_group_to_buffer successfully.\n");
* 		printf("etag = %s\nlast_modified = %s\ncontent_type = %s\ncontent_length = %ld\n", 
* 				result->get_etag(result), result->get_last_modified(result),
* 				result->get_content_type(result), result->get_content_length(result));
* 		printf("buffer = \n%s\n", buffer);
* 	} else {
* 		retinfo = oss_get_error_message_from_retcode(retcode);
* 		printf("error = %s\n", retinfo);
* 	}
* 
* 	// 释放空间 
* 	if(buffer != NULL) {
* 		free(buffer);
* 		buffer = NULL;
* 	}
* 	if(request != NULL) {
* 		get_object_group_request_finalize(request);
* 	}
* 	if(result) {
* 		object_metadata_finalize(result);
* 	}
* 	client_finalize(client);
* 	return 0;
* }
* @endcode
* @subsubsection 获取Object-Group至本地文件
* @code
* #include <ossc/client.h>
* #include <stdio.h>
* 
* static const char *access_id = "ACSfLOiddaOzejOP";   //设置用户帐号
* static const char *access_key = "MUltNpuYqE";  //设置用户密码
* static const char *endpoint = "storage.aliyun.com";    //设置hostname
* 
* // 读取用户指定的Object group到本地文件 
* int main()
* {
* 	unsigned short retcode;			//保存服务器http返回码的解析结果;
* 	const char *retinfo;            //保存通过retcode获得的错误信息
* 
* 	const char *bucket_name = "bucket_example";       //设置bucket_name
* 	const char *key = "a_empty_group_file.dat";             //设置object group的名称
* 	FILE *fp = NULL;
* 	if((fp = fopen("get_object_group", "w+")) == NULL) {
* 		printf("open file failed.\n");
* 		return -1;
* 	}
* 
* 	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
* 	oss_get_object_group_request_t *request = 
* 		get_object_group_request_initialize(bucket_name, key);
* 	oss_object_metadata_t *result = client_get_object_group_to_file(client, request, fp, &retcode);
* 
* 	printf("get_obejct_group_to_file result:\n\n");
* 	if(retcode == OK) {
* 		printf("get_object_group_to_file successfully.\n");
* 		printf("etag = %s\nlast_modified = %s\ncontent_type = %s\ncontent_length = %ld\n", 
* 				result->get_etag(result), result->get_last_modified(result),
* 				result->get_content_type(result), result->get_content_length(result));
* 	} else {
* 		retinfo = oss_get_error_message_from_retcode(retcode);
* 		printf("error = %s\n", retinfo);
* 	}
* 
* 	// 释放空间 
* 	if(request != NULL) {
* 		get_object_group_request_finalize(request);
* 	}
* 	if(result) {
* 		object_metadata_finalize(result);
* 	}
* 	client_finalize(client);
* 	return 0;
* }
* @endcode
*
* @subsection Get02 Get Object Group Index 操作
* @code
* #include <ossc/client.h>
* #include <stdio.h>
* 
* static const char *access_id = "ACSfLOiddaOzejOP";   //设置用户帐号
* static const char *access_key = "MUltNpuYqE";  //设置用户密码
* static const char *endpoint = "storage.aliyun.com";    //设置hostname
* 
* // 返回 Object Group 中的 Object List 信息
* int main()
* {
* 	unsigned short retcode;			//保存服务器http返回码的解析结果;
* 	const char *retinfo;            //保存通过retcode获得的错误信息
* 
* 	const char *bucket_name = "bucket_example";       //设置bucket_name
* 	const char *key = "a_empty_group_file.dat";             //设置object group的名称
* 	int i;
* 
* 	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
* 	oss_get_object_group_index_result_t *result = client_get_object_group_index(client, bucket_name, key, &retcode);
* 
* 	printf("get_obejct_group_index result:\n\n");
* 	if(retcode == OK) {
* 		printf("get_object_group_index successfully.\n");
* 		printf("bucket_name = %s\nkey = %s\netag = %s\nfile_length = %u\n", 
* 				result->get_bucket_name(result), result->get_key(result),
* 				result->get_etag(result), result->get_file_length(result));
* 		printf("============================================\n");
*
* 		for(i = 0; i < result->part_number; i++) {
* 			printf("part [%d] :\n", i);
* 			printf("etag = %s\npart_name = %s\npart_number = %d\npart_size = %u\n", 
* 					(result->group)[i]->etag, (result->group)[i]->part_name,
* 					(result->group)[i]->part_number, (result->group)[i]->part_size);
* 			printf("*****************************************\n");
* 		}
* 	} else {
* 		retinfo = oss_get_error_message_from_retcode(retcode);
* 		printf("error = %s\n", retinfo);
* 	}
* 
* 	// 释放空间 
* 	if(result != NULL) {
* 		if(result->group != NULL) {
* 			for(i = 0; i < result->part_number; i++) {
* 				multipart_object_group_finalize((result->group)[i]);
* 			}
* 			free(result->group);
* 		}
* 		get_object_group_index_result_finalize(result);
* 	}
* 	client_finalize(client);
* 	 return 0;
* }
* @endcode
*
* @subsection Head Head Object Group 操作
* @code
* #include <ossc/client.h>
* #include <stdio.h>
* 
* static const char *access_id = "ACSfLOiddaOzejOP";   //设置用户帐号
* static const char *access_key = "MUltNpuYqE";  //设置用户密码
* static const char *endpoint = "storage.aliyun.com";    //设置hostname
* 
* // Head Object Group 返回用户指定的 Object Group 信息, 不返回文件内容。
* int main()
* {
* 	unsigned short retcode;			//保存服务器http返回码的解析结果;
* 	const char *retinfo;            //保存通过retcode获得的错误信息
* 
* 	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
* 	const char *bucket_name = "bucket_example";       //设置bucket_name
* 	const char *key = "a_group_file.dat";             //设置object group的名称
* 
* 	oss_get_object_group_request_t *request = get_object_group_request_initialize(bucket_name, key);
* 	request->set_modified_since_constraint(request, "Sat, 20 Sep 2012 23:23:23 GMT");
* 	oss_object_metadata_t *result = client_head_object_group(client, request, &retcode);
* 
* 	printf("head_obejct_group result:\n\n");
* 	if(retcode == OK) {
* 		printf("head_object_group successfully.\n");
* 		printf("content-length = %ld\ncontent-type = %s\nlast-modified = %s\n",
* 				result->get_content_length(result),
* 				result->get_content_type(result),
* 				result->get_last_modified(result));
* 	} else {
* 		retinfo = oss_get_error_message_from_retcode(retcode);
* 		printf("error = %s\n", retinfo);
* 	}
* 
* 	// 释放空间 
* 	if(request != NULL) {
* 		get_object_group_request_finalize(request);
* 	}
* 	if(result) {
* 		object_metadata_finalize(result);
* 	}
* 	client_finalize(client);
* 	return 0;
* }
* @endcode
*
* @subsection Delete Delete Object Group 操作
* @code
* #include <ossc/client.h>
* #include <stdio.h>
* 
* static const char *access_id = "ACSfLOiddaOzejOP";   //设置用户帐号
* static const char *access_key = "MUltNpuYqE";  //设置用户密码
* static const char *endpoint = "storage.aliyun.com";    //设置hostname
* 
* //Delete Object Group 删除用户指定的 Object Group 信息
* int main()
* {
* 	unsigned short retcode;			//保存服务器http返回码的解析结果;
* 	const char *retinfo;            //保存通过retcode获得的错误信息
* 
* 	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
* 	const char *bucket_name = "bucket_example";       //设置bucket_name
* 	const char *key = "a_group_file.dat";             //设置object group的名称
* 
* 	client_delete_object_group(client, bucket_name, key, &retcode);
* 
* 	printf("delete_obejct_group result:\n\n");
* 	if(retcode == NO_CONTENT) {
* 		printf("delete_object_group successfully.\n");
* 	} else {
* 		retinfo = oss_get_error_message_from_retcode(retcode);
* 		printf("error = %s\n", retinfo);
* 	}
* 
* 	//释放空间 
* 	client_finalize(client);
* 	return 0;
* }
* @endcode
*/


