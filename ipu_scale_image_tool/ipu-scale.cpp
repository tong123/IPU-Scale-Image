#include "ipu-scale.h"
#include <QDebug>
#include <QImage>
#include <string.h>
#include <QFile>
IPUScale::IPUScale(QObject *parent) : QObject(parent)
{
//    scale_test( );
    mn_zoom_out_size = 1.0;
//    scale_image( "/media/sd-mmcblk2p1/ipu_main/images/img.irp.jpg", 640, 480, "/media/sd-mmcblk2p1/ipu_main/images/img_320_240.irp.jpg", 320, 240 );
//    scale_image( "/media/sd-mmcblk2p1/ipu_main/images/img.irp.jpg", 640, 480, "/media/sd-mmcblk2p1/ipu_main/images/img_1000_1000.irp.jpg", 1000, 1000 );
}

IPUScale::~IPUScale()
{

}

void IPUScale::reset_zoom_size()
{
    mn_zoom_out_size = 1.0;
}

void IPUScale::get_cpu_info()
{
    QFile file("cpu.txt");
    if( file.remove() ) {
        qDebug()<<"remove file success!";
    }
    system("top -n 1 | grep Cpu | cut -d \",\" -f 1 | cut -d \":\" -f 2 > cpu.txt");
    if( file.exists() ) {
        if( file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
            QByteArray ba = file.readAll();
            QString s_cpu_info = ba;
            qDebug()<<"get_cpu_info: "<<s_cpu_info.simplified();
            file.close();
        }

    }
}

void IPUScale::zoom_in( QString s_img_path )//larger
{
    if( mn_zoom_out_size<=10 ) {
        qDebug()<<s_img_path;
        QString s_src_img_path = s_img_path;
        QImage img( s_src_img_path );
        int n_src_width = img.width();
        int n_src_height = img.height();
        mn_zoom_out_size = mn_zoom_out_size+0.1;
        int n_scale_width = ( 10*(n_src_width*mn_zoom_out_size)+5 )/10;
        int n_scale_height = ( 10*(n_src_height*mn_zoom_out_size)+5)/10;
//        QString s_des_img_path = QString("%1%2%3").arg(s_src_img_path.mid(0, s_src_img_path.lastIndexOf("/"))).arg("/zoom").arg(s_src_img_path.mid(s_src_img_path.indexOf("."), s_src_img_path.length()));
        QString s_des_img_path = QString("%1%2_%3_%4%5").arg(s_src_img_path.mid(0, s_src_img_path.lastIndexOf("/"))).arg( s_src_img_path.mid( s_src_img_path.lastIndexOf("/"), s_src_img_path.indexOf(".")-s_src_img_path.lastIndexOf("/") ) ).arg(n_scale_width).arg(n_scale_height).arg( s_src_img_path.mid(s_src_img_path.indexOf("."), s_src_img_path.length()));
        qDebug()<<"s_des_img_path: "<<s_des_img_path;

        qDebug()<<"width, height: "<<n_src_width<<n_src_height;
        if( n_src_width==n_scale_width && n_src_height==n_scale_height ) {
            emit changed_image_show( QString("file://%1").arg(s_src_img_path) );
        } else {
            int n_ret = scale_image( s_src_img_path, n_src_width, n_src_height, s_des_img_path, n_scale_width, n_scale_height );
            if( n_ret<0 ) {
                mn_zoom_out_size = mn_zoom_out_size-0.1;
            }
        }
    }
//    get_cpu_info();

}

void IPUScale::zoom_out( QString s_img_path )//smaller
{
    if( mn_zoom_out_size>=0.1 ) {
        qDebug()<<s_img_path;
        QString s_src_img_path = s_img_path;
        QImage img( s_src_img_path );
        int n_src_width = img.width();
        int n_src_height = img.height();
        mn_zoom_out_size = mn_zoom_out_size-0.1;
        int n_scale_width = ( 10*(n_src_width*mn_zoom_out_size)+5 )/10;
        int n_scale_height = ( 10*(n_src_height*mn_zoom_out_size)+5)/10;
        qDebug()<<"mn_zoom_out_size: "<<mn_zoom_out_size<<n_scale_width<<n_scale_height<<n_src_width*mn_zoom_out_size;
        QString s_des_img_path = QString("%1%2_%3_%4%5").arg(s_src_img_path.mid(0, s_src_img_path.lastIndexOf("/"))).arg( s_src_img_path.mid( s_src_img_path.lastIndexOf("/"), s_src_img_path.indexOf(".")-s_src_img_path.lastIndexOf("/") ) ).arg(n_scale_width).arg(n_scale_height).arg( s_src_img_path.mid(s_src_img_path.indexOf("."), s_src_img_path.length()));
        qDebug()<<"s_des_img_path: "<<s_des_img_path;
        qDebug()<<"width, height: "<<n_src_width<<n_src_height;
        if( n_src_width==n_scale_width && n_src_height==n_scale_height ) {
            emit changed_image_show( QString("file://%1").arg(s_src_img_path) );
        } else {
            int n_ret = scale_image( s_src_img_path, n_src_width, n_src_height, s_des_img_path, n_scale_width, n_scale_height );
            if( n_ret<0 ) {
                mn_zoom_out_size = mn_zoom_out_size+0.1;
            }
        }
    }
//    get_cpu_info();

}

int IPUScale::scale_image( QString s_src_file, unsigned int n_src_width, unsigned int n_src_height,
                           QString s_des_file, unsigned int n_des_width, unsigned int n_des_height )
{
#ifdef YOCT_SYS
    struct ipu_task task;
    struct timeval begin, end;
    int sec, usec, run_time = 0;
    int fd_ipu = 0;   // IPU file descriptor
    int isize = 0, osize = 0, ret = 0;    // input size output size
    void *inbuf = NULL;
    void *outbuf = NULL;

    memset(&task, 0, sizeof(task));
    // Input image size and format
    task.input.width    = n_src_width;
    task.input.height   = n_src_height;
    task.input.format   = v4l2_fourcc('R', 'G', 'B', 'A');
    // Output image size and format
    task.output.width   = n_des_width;
    task.output.height  = n_des_height;
    task.output.format  = v4l2_fourcc('R', 'G', 'B', 'A');

    QImage src_img = QImage( n_src_width, n_src_height, QImage::Format_RGB32 );
    QImage des_img;
    QFile file(s_des_file);
    bool b_save_status;
    src_img.load( s_src_file );
    int img_size = src_img.byteCount();
    unsigned char *buf = src_img.bits();
    qDebug()<<"img_size: "<<img_size<<src_img.size();
    qDebug()<<"open IPU device!";
    fd_ipu = open("/dev/mxc_ipu", O_RDWR, 0);
    if (fd_ipu < 0) {
        printf("open ipu dev fail\n");
        ret = -1;
        goto done;
    }
    qDebug()<<"according to format: "<<task.input.width<<task.input.height<<fmt_to_bpp(task.input.format);
    qDebug()<<"scale_image: "<<n_des_width<<n_des_height;
//    isize = task.input.paddr = task.input.width * task.input.height* fmt_to_bpp(task.input.format)/8;
    isize = task.input.paddr = img_size;

    // Allocate contingous physical memory for input image
    // input.paddr contains the amount needed
    // this value will be replaced with physical address on success
    ret = ioctl(fd_ipu, IPU_ALLOC, &task.input.paddr);
    if (ret < 0) {
        printf("ioctl IPU_ALLOC fail: (errno = %d)\n", errno);
        goto done;
    }

    // Create memory map and obtain the allocated memory virtual address
    inbuf = mmap(0, isize, PROT_READ | PROT_WRITE, MAP_SHARED, fd_ipu, task.input.paddr);
    if (!inbuf) {
        printf("mmap fail\n");
        ret = -1;
        goto done;
    }

    // Allocate memory for output image
    osize = task.output.paddr = task.output.width * task.output.height * fmt_to_bpp(task.output.format)/8;
    ret = ioctl(fd_ipu, IPU_ALLOC, &task.output.paddr);
    if (ret < 0) {
        printf("ioctl IPU_ALLOC fail\n");
        goto done;
    }

    // Create memory map for output image
    outbuf = mmap(0, osize, PROT_READ | PROT_WRITE,
    MAP_SHARED, fd_ipu, task.output.paddr);
    if (!outbuf) {
        printf("mmap fail\n");
        ret = -1;
        goto done;
    }
    memcpy( inbuf, buf, isize );

    gettimeofday(&begin, NULL);
    // Perform the rotation
    ret = ioctl(fd_ipu, IPU_QUEUE_TASK, &task);
    if (ret < 0) {
        printf("ioct IPU_QUEUE_TASK fail\n");
        goto done;
    }
    gettimeofday(&end, NULL);

    sec = end.tv_sec - begin.tv_sec;
    usec = end.tv_usec - begin.tv_usec;
    if (usec < 0) {
        sec--;
        usec = usec + 1000000;
    }
    run_time = (sec * 1000000) + usec;

    // Write resized image to output file
    des_img = QImage( (const uchar*)outbuf, n_des_width, n_des_height, QImage::Format_RGB32 );
    qDebug()<<des_img.byteCount()<<des_img.size()<<osize;
    file.remove();
    b_save_status = des_img.save( s_des_file );
    qDebug()<<"save status: "<<b_save_status;

    printf("Resize time: %d usecs\n", run_time);
    emit changed_image_show( QString("file://%1").arg(s_des_file) );
done:

    if (outbuf)
        munmap(outbuf, osize);
    if (task.output.paddr)
        ioctl(fd_ipu, IPU_FREE, &task.output.paddr);
    if (inbuf)
        munmap(inbuf, isize);
    if (task.input.paddr)
        ioctl(fd_ipu, IPU_FREE, &task.input.paddr);
    if (fd_ipu)
        close(fd_ipu);

    return ret;
#endif
}


int IPUScale::scale_test( )
{
#ifdef YOCT_SYS
    struct ipu_task task;

    FILE * file_in = NULL;
    FILE * file_out = NULL;

    struct timeval begin, end;
    int sec, usec, run_time = 0;

    int fd_ipu = 0;   // IPU file descriptor
    int isize = 0;    // input size
    int osize = 0;    // output size

    void *inbuf = NULL;
    void *outbuf = NULL;

    int ret = 0;

    memset(&task, 0, sizeof(task));

    // Input image size and format
    task.input.width    = 1024;
    task.input.height   = 768;
    task.input.format   = v4l2_fourcc('R', 'G', 'B', 'A');

    // Output image size and format
    task.output.width   = 600;
    task.output.height  = 600;
    task.output.format  = v4l2_fourcc('R', 'G', 'B', '4');
    QImage img = QImage( 1024, 768, QImage::Format_RGB32 );
//    QImage out_img = QImage( 1024, 768, QImage::Format_RGB32 );
    QImage out_img;
//    img.load( "/media/sd-mmcblk2p1/ipu_main/images/freescale_1024x768.png" );
    img.load( "/media/sd-mmcblk2p1/ipu_main/images/freescale_1024x768.png" );

    int img_size = img.byteCount();
    unsigned char *buf = img.bits();
    qDebug()<<"img_size: "<<img_size<<img.size();
//    qDebug()<<"loadFromData: "<<out_img.loadFromData( buf, img_size, "PNG" );
//    qDebug()<<"out_img_size: "<<out_img.byteCount()<<out_img.size();
    // Open the raw im age
    if ((file_in = fopen("/media/sd-mmcblk2p1/ipu_main/images/freescale_1024x768_yuyv422.raw", "rb")) < 0) {
        printf("Unable to open freescale_1024x768.raw\n");
        ret = -1;
        goto done;
    }
    qDebug()<<"open IPU device!";
    // Open IPU device
    fd_ipu = open("/dev/mxc_ipu", O_RDWR, 0);
    if (fd_ipu < 0) {
        printf("open ipu dev fail\n");
        ret = -1;
        goto done;
    }

    // Calculate input size from image dimensions and bits-per-pixel
    // according to format
    qDebug()<<"according to format: "<<task.input.width<<task.input.height<<fmt_to_bpp(task.input.format);
//    isize = task.input.paddr = task.input.width * task.input.height* fmt_to_bpp(task.input.format)/8;
    isize = task.input.paddr = img_size;

    // Allocate contingous physical memory for input image
    // input.paddr contains the amount needed
    // this value will be replaced with physical address on success
    ret = ioctl(fd_ipu, IPU_ALLOC, &task.input.paddr);
    if (ret < 0) {
        printf("ioctl IPU_ALLOC fail: (errno = %d)\n", errno);
        goto done;
    }

    // Create memory map and obtain the allocated memory virtual address
    inbuf = mmap(0, isize, PROT_READ | PROT_WRITE,
    MAP_SHARED, fd_ipu, task.input.paddr);
    if (!inbuf) {
        printf("mmap fail\n");
        ret = -1;
        goto done;
    }

    // Allocate memory for output image
    osize = task.output.paddr = task.output.width * task.output.height * fmt_to_bpp(task.output.format)/8;
//    osize = task.output.paddr = 59608;
    ret = ioctl(fd_ipu, IPU_ALLOC, &task.output.paddr);
    if (ret < 0) {
        printf("ioctl IPU_ALLOC fail\n");
        goto done;
    }

    // Create memory map for output image
    outbuf = mmap(0, osize, PROT_READ | PROT_WRITE,
    MAP_SHARED, fd_ipu, task.output.paddr);
    if (!outbuf) {
        printf("mmap fail\n");
        ret = -1;
        goto done;
    }

    // Open output file for writing
    if ((file_out = fopen("output_file.raw", "wb")) < 0) {
        printf("Cannot open output file");
        ret = -1;
        goto done;
    }

    // Read input image
//    ret = fread(inbuf, 1, isize, file_in);
    memcpy( inbuf, buf, isize );
    qDebug()<<ret<<isize;
//    if (ret < isize) {
//        ret = 0;
//        printf("Cannot read enough data from input file\n");
//        goto done;
//    }

    gettimeofday(&begin, NULL);

    // Perform the rotation
    ret = ioctl(fd_ipu, IPU_QUEUE_TASK, &task);
    if (ret < 0) {
        printf("ioct IPU_QUEUE_TASK fail\n");
        goto done;
    }

    gettimeofday(&end, NULL);
    sec = end.tv_sec - begin.tv_sec;
    usec = end.tv_usec - begin.tv_usec;
    if (usec < 0) {
        sec--;
        usec = usec + 1000000;
    }
    run_time = (sec * 1000000) + usec;

    // Write resized image to output file
//    out_img.loadFromData( (const uchar*)outbuf, osize );
//    out_img = out_img.fromData((const uchar*)outbuf, osize);
    out_img = QImage( (const uchar*)outbuf, 600, 600, QImage::Format_RGB32 );
    qDebug()<<out_img.byteCount()<<out_img.size()<<osize;
    qDebug()<<"save status: "<<out_img.save( "freescale_800.png" );
    ret = fwrite(outbuf, 1, osize, file_out);
    if (ret < osize) {
        ret = -1;
        printf("Cannot write enough data into output file\n");
        goto done;
    }

    printf("Resize time: %d usecs\n", run_time);

done:
    if (file_out)
        fclose(file_out);
    if (outbuf)
        munmap(outbuf, osize);
    if (task.output.paddr)
        ioctl(fd_ipu, IPU_FREE, &task.output.paddr);
    if (inbuf)
        munmap(inbuf, isize);
    if (task.input.paddr)
        ioctl(fd_ipu, IPU_FREE, &task.input.paddr);
    if (fd_ipu)
        close(fd_ipu);
    if (file_in)
        fclose(file_in);

    return ret;
#endif
}

unsigned int IPUScale::fmt_to_bpp( unsigned int pixelformat )
{
#ifdef YOCT_SYS

    unsigned int bpp;

        switch (pixelformat)
        {
                case IPU_PIX_FMT_RGB565:
               /*interleaved 422*/
                case IPU_PIX_FMT_YUYV:
                case IPU_PIX_FMT_UYVY:
                /*non-interleaved 422*/
                case IPU_PIX_FMT_YUV422P:
                case IPU_PIX_FMT_YVU422P:
                        bpp = 16;
                        break;
                case IPU_PIX_FMT_BGR24:
                case IPU_PIX_FMT_RGB24:
                case IPU_PIX_FMT_YUV444:
                case IPU_PIX_FMT_YUV444P:
                        bpp = 24;
                        break;
                case IPU_PIX_FMT_BGR32:
                case IPU_PIX_FMT_BGRA32:
                case IPU_PIX_FMT_RGB32:
                case IPU_PIX_FMT_RGBA32:
                case IPU_PIX_FMT_ABGR32:
                        bpp = 32;
                        break;
                /*non-interleaved 420*/
                case IPU_PIX_FMT_YUV420P:
                case IPU_PIX_FMT_YVU420P:
                case IPU_PIX_FMT_YUV420P2:
                case IPU_PIX_FMT_NV12:
    case IPU_PIX_FMT_TILED_NV12:
                        bpp = 12;
                        break;
                default:
                        bpp = 8;
                        break;
        }
        return bpp;
#endif
}
