#ifndef FILECUSTOMDIALOG_H
#define FILECUSTOMDIALOG_H

#include <QObject>
#include <QList>
#include <QStringList>


#define FOLDER_SEPARATOR "/"

class FileCustomDialog : public QObject
{
    Q_OBJECT
public:
    explicit FileCustomDialog(QObject *parent = 0);
    Q_INVOKABLE int get_file_folder_number() { return ms_file_folder_name_list.size(); }
    Q_INVOKABLE QString get_file_folder_name( int n_indx ) { return n_indx < ms_file_folder_name_list.size()?ms_file_folder_name_list.at( n_indx ):""; }
    Q_INVOKABLE void show_default_name( bool b_import_shoot_result, bool b_vi_result = false );
    Q_INVOKABLE void show_sd_dir();
    Q_INVOKABLE void show_file_system_dir();
    Q_INVOKABLE void show_home_dir();
    Q_INVOKABLE bool show_upper_level();
    Q_INVOKABLE bool show_to_folder( QString s_name );
    Q_INVOKABLE void show_this_folder( QString s_folder_name );
    Q_INVOKABLE QString get_current_path( );

private:
    QStringList ms_file_folder_name_list;
    QString ms_curr_path;

};

#endif // QMLPORT_H
