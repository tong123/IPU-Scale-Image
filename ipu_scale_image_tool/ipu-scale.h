#ifndef IPUSCALE_H
#define IPUSCALE_H

#include <QObject>
#ifdef YOCT_SYS
#include "ipu.h"
#include "mxcfb.h"
#endif
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

class IPUScale : public QObject
{
    Q_OBJECT
public:
    explicit IPUScale(QObject *parent = 0);
    ~IPUScale();
    int scale_test( );
    int scale_image( QString s_src_file, unsigned int n_src_width, unsigned int n_src_height,
                     QString s_des_file, unsigned int n_des_width, unsigned int n_des_height );

    unsigned int fmt_to_bpp( unsigned int pixelformat );
    Q_INVOKABLE void zoom_in( QString s_img_path );
    Q_INVOKABLE void zoom_out( QString s_img_path );
    Q_INVOKABLE void reset_zoom_size( );
    Q_INVOKABLE void get_cpu_info( );


signals:
    void changed_image_show( QString s_string );
public slots:
private:
    float mn_zoom_out_size;
};

#endif // IPUSCALE_H
