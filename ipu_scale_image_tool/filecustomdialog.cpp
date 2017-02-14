#include "filecustomdialog.h"
#include <QDebug>
#include <QStandardPaths>
#include <QDir>
#include <QApplication>
#include <QFileInfoList>
#include <QDir>

FileCustomDialog::FileCustomDialog(QObject *parent) : QObject(parent)
{

}

void FileCustomDialog::show_default_name( bool b_import_shoot_result, bool b_vi_result )
{
    QString s_folder_name;
    s_folder_name = "/media/sd-mmcblk2p1/";
    show_this_folder( s_folder_name );
}


void FileCustomDialog::show_sd_dir( )
{
    show_this_folder("/media/sd-mmcblk2p1/");
}

void FileCustomDialog::show_file_system_dir( )
{
    show_this_folder( "/" );
}

void FileCustomDialog::show_home_dir()
{
    show_this_folder( "/home" );
}

void FileCustomDialog::show_this_folder( QString s_folder_name )
{
    QDir dir( s_folder_name );

    ms_curr_path = s_folder_name;
    ms_file_folder_name_list.clear();

    QStringList s_name_filter_list;

    s_name_filter_list << "*.jpg"<< "*.png"<<"*.bmp";
    QStringList s_list = dir.entryList( s_name_filter_list, QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot );
    for( int i=0; i<s_list.size(); ++i ) {
        QString s_tmp = s_list.at( i );
        int n_indx = s_tmp.lastIndexOf( FOLDER_SEPARATOR );
        if( n_indx != -1 ) {
            s_tmp = s_tmp.mid( n_indx + 1 );
        }
        ms_file_folder_name_list << s_tmp;
    }
    //qDebug() << "FileCustomDialog::show_this_folder s_folder_name=" << s_folder_name << " ms_file_folder_name_list=" << ms_file_folder_name_list.size();
}


bool FileCustomDialog::show_upper_level( )
{
    if( ms_curr_path.isEmpty() ) {
        return false;
    }
    QDir dir( ms_curr_path );
    if( dir.cdUp() ) {
        show_this_folder( dir.canonicalPath() );
        return true;
    }
    return false;
}

bool FileCustomDialog::show_to_folder( QString s_name )
{
    qDebug() << "FileCustomDialog::show_to_folder ms_curr_path=" << ms_curr_path << " s_name=" << s_name;
    if( ms_curr_path.isEmpty() ) {
        show_this_folder( s_name );
    } else {
        QString s_full_name = ms_curr_path;
        if( s_full_name.endsWith( FOLDER_SEPARATOR ) ) {
            s_full_name += s_name;
        } else {
            s_full_name += FOLDER_SEPARATOR + s_name;
        }

        QDir dir( s_full_name );
        if( !dir.exists() ) {
            return false;
        }

        show_this_folder( s_full_name );
    }

    return true;
}

QString FileCustomDialog::get_current_path( )
{
    return ms_curr_path;
}
