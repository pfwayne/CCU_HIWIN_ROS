#ifndef HIWIN_RVIZ_PLUGIN_H
#define HIWIN_RVIZ_PLUGIN_H

#include <ros/ros.h>
#include <ros/console.h>
#include <rviz/panel.h>

class QLineEdit;
class QPushButton;
class QTabWidget;
namespace hiwin_rviz_plugin
{
  class  HiwinPannel: public rviz::Panel
  {
      Q_OBJECT
      public:
        HiwinPannel( QWidget* parent = 0 );
        virtual void load( const rviz::Config& config );
        virtual void save( rviz::Config config ) const;

      //public Q_SLOTS:
        //void setTopic( const QString& topic );

      protected Q_SLOTS:
        //void sendVel();
        void updatetab1_X();
        void updatetab1_Y();
        void updatetab1_Z();
        void updatetab1_A();
        void updatetab1_B();
        void updatetab1_C();
        void ptppos();
        void ptprelpos();
        void ptpaxis();
        void ptprelaxis();
        void rotate();
        void refresh();
        void home();
        

      protected:
        QLineEdit* output_topic_editor_;
        QString output_topic_;
        QLineEdit* output_topic_editor_1;
        QString output_topic_1;
        QLineEdit* output_topic_editor_2;
        QString output_topic_2;
        QLineEdit* output_topic_editor_3;
        QString output_topic_3;
        QLineEdit* output_topic_editor_4;
        QString output_topic_4;
        QLineEdit* output_topic_editor_5;
        QString output_topic_5;

        QTabWidget *tabWidget;
        

        QPushButton *btn1;
        QPushButton *btn2;
        QPushButton *btn3;
        QPushButton *btn4;
        QPushButton *btn5;
        QPushButton *refreshbtn;
        QPushButton *homebtn;

        ros::Publisher velocity_publisher_;
        ros::NodeHandle nh_;

        double tab1_X;
        double tab1_Y;
        double tab1_Z;
        double tab1_A;
        double tab1_B;
        double tab1_C;


  };
}

#endif