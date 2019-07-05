#include <stdio.h>
#include <string> 
#include <QPainter>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QPushButton>
#include <QTabWidget>

#include "HRSDK.h"

#include <geometry_msgs/Twist.h>
#include <QDebug>

#include "hiwin_rviz_plugin.h"

#pragma comment(lib,"HRSDK.lib")



void __stdcall callBack(uint16_t, uint16_t, uint16_t*, int) {

}


namespace hiwin_rviz_plugin{
  HiwinPannel::HiwinPannel( QWidget* parent ): rviz::Panel( parent ), tab1_X( 100 ),tab1_Y( 0 ),tab1_Z( 0 ),tab1_A( 0 ),tab1_B( 0 ),tab1_C( 0 ){
    tabWidget = new QTabWidget(this);

    double return_pos[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    
    HROBOT device_id = Connect("169.254.128.198", 1, callBack);
    get_current_position(device_id,return_pos);
    
    tab1_X = return_pos[0];
    tab1_Y = return_pos[1];
    tab1_Z = return_pos[2];
    tab1_A = return_pos[3];
    tab1_B = return_pos[4];
    tab1_C = return_pos[5];
    Close(device_id);
    /////////////////tab1
    auto tab1 = new QWidget;
    QVBoxLayout* tab1_vlayout = new QVBoxLayout;
    QVBoxLayout* tab1_v2ayout = new QVBoxLayout;
    tab1_vlayout->addWidget( new QLabel( "X:" ));
    output_topic_editor_ = new QLineEdit;
    output_topic_editor_->setText(QString::fromStdString(std::to_string(tab1_X)));
    tab1_vlayout->addWidget( output_topic_editor_ );
    tab1_vlayout->addWidget( new QLabel( "Y:" ));
    output_topic_editor_1 = new QLineEdit;
    output_topic_editor_1->setText(QString::fromStdString(std::to_string(tab1_Y)));
    tab1_vlayout->addWidget( output_topic_editor_1 );
    tab1_vlayout->addWidget( new QLabel( "Z:" ));
    output_topic_editor_2 = new QLineEdit;
    output_topic_editor_2->setText(QString::fromStdString(std::to_string(tab1_Z)));
    tab1_vlayout->addWidget( output_topic_editor_2 );
    tab1_v2ayout->addWidget( new QLabel( "A:" ));
    output_topic_editor_3 = new QLineEdit;
    output_topic_editor_3->setText(QString::fromStdString(std::to_string(tab1_A)));
    tab1_v2ayout->addWidget( output_topic_editor_3 );
    tab1_v2ayout->addWidget( new QLabel( "B:" ));
    output_topic_editor_4 = new QLineEdit;
    output_topic_editor_4->setText(QString::fromStdString(std::to_string(tab1_B)));
    tab1_v2ayout->addWidget( output_topic_editor_4 );
    tab1_v2ayout->addWidget( new QLabel( "C:" ));
    output_topic_editor_5 = new QLineEdit;
    output_topic_editor_5->setText(QString::fromStdString(std::to_string(tab1_C)));
    tab1_v2ayout->addWidget( output_topic_editor_5 );

    QHBoxLayout* tab1_bhlayout = new QHBoxLayout; 
    refreshbtn = new QPushButton(tr("Refresh"),this);   
    refreshbtn->setText(tr("Refresh"));              
    refreshbtn->setFont(QFont("Times",12));    
    tab1_bhlayout->addWidget( refreshbtn );

    homebtn = new QPushButton(tr("Home"),this);   
    homebtn->setText(tr("Home"));              
    homebtn->setFont(QFont("Times",12));    
    tab1_bhlayout->addWidget( homebtn );

    QHBoxLayout* tab1_bh2layout = new QHBoxLayout; 
    btn1 = new QPushButton(tr("PTPPOS"),this);   
    btn1->setText(tr("PTPPOS"));              
    btn1->setFont(QFont("Times",12)); 
    tab1_bh2layout->addWidget( btn1 );
    btn2 = new QPushButton(tr("PTPRELPOS"),this);   
    btn2->setText(tr("PTPRELPOS"));              
    btn2->setFont(QFont("Times",12));
    tab1_bh2layout->addWidget( btn2 );

    QHBoxLayout* tab1_bh3layout = new QHBoxLayout; 
    btn3 = new QPushButton(tr("PTPAXIS"),this);   
    btn3->setText(tr("PTPAXIS"));              
    btn3->setFont(QFont("Times",12)); 
    tab1_bh3layout->addWidget( btn3 );
    btn4 = new QPushButton(tr("PTPRELAXIS"),this);   
    btn4->setText(tr("PTPRELAXIS"));              
    btn4->setFont(QFont("Times",12));
    tab1_bh3layout->addWidget( btn4 );

    QHBoxLayout* tab1_bh4layout = new QHBoxLayout; 
    btn5 = new QPushButton(tr("ROTATE"),this);   
    btn5->setText(tr("ROTATE"));              
    btn5->setFont(QFont("Times",12)); 
    tab1_bh4layout->addWidget( btn5 );

    QVBoxLayout* tab1_v3ayout = new QVBoxLayout;
    tab1_v3ayout->addLayout( tab1_bhlayout );   
    tab1_v3ayout->addLayout( tab1_bh2layout );
    tab1_v3ayout->addLayout( tab1_bh3layout );
    tab1_v3ayout->addLayout( tab1_bh4layout );
    QHBoxLayout* tab1_h1ayout = new QHBoxLayout;
    tab1_h1ayout->addLayout( tab1_vlayout);
    tab1_h1ayout->addLayout( tab1_v2ayout);
    QVBoxLayout* totalayout = new QVBoxLayout;
    totalayout->addLayout( tab1_h1ayout);
    totalayout->addLayout( tab1_v3ayout);
    tab1->setLayout(totalayout);


    tabWidget->addTab(tab1, tr("Control"));
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tabWidget);
    setLayout( mainLayout );
  

    //QTimer* output_timer = new QTimer( this );
    connect( output_topic_editor_, SIGNAL( editingFinished() ), this, SLOT( updatetab1_X() ));
    connect( output_topic_editor_1, SIGNAL( editingFinished() ), this, SLOT( updatetab1_Y() ));
    connect( output_topic_editor_2, SIGNAL( editingFinished() ), this, SLOT( updatetab1_Z() ));
    connect( output_topic_editor_3, SIGNAL( editingFinished() ), this, SLOT( updatetab1_A() ));
    connect( output_topic_editor_4, SIGNAL( editingFinished() ), this, SLOT( updatetab1_B() ));
    connect( output_topic_editor_5, SIGNAL( editingFinished() ), this, SLOT( updatetab1_C() ));
    connect( btn1, SIGNAL( released() ), this, SLOT( ptppos() ));
    connect( btn2, SIGNAL( released() ), this, SLOT( ptprelpos() ));
    connect( btn3, SIGNAL( released() ), this, SLOT( ptpaxis() ));
    connect( btn4, SIGNAL( released() ), this, SLOT( ptprelaxis() ));
    connect( btn5, SIGNAL( released() ), this, SLOT( rotate() ));
    connect( refreshbtn, SIGNAL( released() ), this, SLOT( refresh() ));
    connect( homebtn, SIGNAL( released() ), this, SLOT( home() ));
    //output_timer->start( 100 );
  }


  void HiwinPannel::updatetab1_X()
  {
      QString temp_string = output_topic_editor_->text();
      double lin = temp_string.toDouble();
      tab1_X = lin;
      output_topic_editor_->setText(QString::fromStdString(std::to_string(tab1_X)));
  }
  void HiwinPannel::updatetab1_Y()
  {
      QString temp_string = output_topic_editor_1->text();
      double lin = temp_string.toDouble();
      tab1_Y = lin;
      output_topic_editor_1->setText(QString::fromStdString(std::to_string(tab1_Y)));
  }
  void HiwinPannel::updatetab1_Z()
  {   QString temp_string = output_topic_editor_2->text();
      double lin = temp_string.toDouble();
      tab1_Z = lin;
      output_topic_editor_2->setText(QString::fromStdString(std::to_string(tab1_Z)));
    
  }
   void HiwinPannel::updatetab1_A()
  {
      QString temp_string = output_topic_editor_3->text();
      double lin = temp_string.toDouble();
      tab1_A = lin;
      output_topic_editor_3->setText(QString::fromStdString(std::to_string(tab1_A)));
  }
   void HiwinPannel::updatetab1_B()
  {
      QString temp_string = output_topic_editor_4->text();
      double lin = temp_string.toDouble();
      tab1_B = lin;
      output_topic_editor_4->setText(QString::fromStdString(std::to_string(tab1_B)));
  }
   void HiwinPannel::updatetab1_C()
  {
      QString temp_string = output_topic_editor_5->text();
      double lin = temp_string.toDouble();
      tab1_C = lin;
      output_topic_editor_5->setText(QString::fromStdString(std::to_string(tab1_C)));
  }
   
   void HiwinPannel::ptppos()
  {
    HROBOT device_id = Connect("169.254.128.198", 1, callBack);
    if (device_id < 0)
    {
        
        ROS_WARN("Failed Connection.\n");
    }
    else
    {
      set_override_ratio(device_id, 100);

        if (get_motor_state(device_id) == 0)
        {
            set_motor_state(device_id, 1); // Servo on
                                           // HiwinMoveitAction HiwinMoveitAction("hiwin_6dot/follow_joint_trajectory");
        }
      
      double input[6]={0, 270, 210, 180, 0, 90};
      ptp_pos(device_id, 1, input);
      ROS_WARN("Failed Excution.\n");
    }
    Close(device_id);
     
  }
  void HiwinPannel::ptprelpos()
  {
    HROBOT device_id = Connect("169.254.128.198", 1, callBack);
    if (device_id < 0)
    {
        
        ROS_WARN("Failed Connection.\n");
    }
    else
    {
      set_override_ratio(device_id, 100);

        if (get_motor_state(device_id) == 0)
        {
            set_motor_state(device_id, 1); // Servo on
                                           // HiwinMoveitAction HiwinMoveitAction("hiwin_6dot/follow_joint_trajectory");
        }
      
      double input[6]={ 100, 100, 100, 0, 0, 0};
      ptp_rel_pos(device_id, 1, input);
      ROS_WARN("Failed Excution.\n");
    }
    Close(device_id);
     
  }
  void HiwinPannel::ptpaxis()
  {
    HROBOT device_id = Connect("169.254.128.198", 1, callBack);
    if (device_id < 0)
    {
        
        ROS_WARN("Failed Connection.\n");
    }
    else
    {
      set_override_ratio(device_id, 100);

        if (get_motor_state(device_id) == 0)
        {
            set_motor_state(device_id, 1); // Servo on
                                           // HiwinMoveitAction HiwinMoveitAction("hiwin_6dot/follow_joint_trajectory");
        }
      
      double input[6]={ 0, -45, 0, 0, 45, 90};
      ptp_axis(device_id, 1, input);
      ROS_WARN("Failed Excution.\n");
    }
    Close(device_id);
     
  }

  void HiwinPannel::ptprelaxis()
  {
    HROBOT device_id = Connect("169.254.128.198", 1, callBack);
    if (device_id < 0)
    {
        
        ROS_WARN("Failed Connection.\n");
    }
    else
    {
      set_override_ratio(device_id, 100);

        if (get_motor_state(device_id) == 0)
        {
            set_motor_state(device_id, 1); // Servo on
                                           // HiwinMoveitAction HiwinMoveitAction("hiwin_6dot/follow_joint_trajectory");
        }
      
      double input[6]={ 0, 30, -30, 0, 0, 0};
      ptp_rel_axis(device_id, 1, input);
      ROS_WARN("Failed Excution.\n");
    }
    Close(device_id);
     
  }

    void HiwinPannel::rotate()
  {
    HROBOT device_id = Connect("169.254.128.198", 1, callBack);
    if (device_id < 0)
    {
        
        ROS_WARN("Failed Connection.\n");
    }
    else
    {
      set_override_ratio(device_id, 100);

        if (get_motor_state(device_id) == 0)
        {
            set_motor_state(device_id, 1); // Servo on
                                           // HiwinMoveitAction HiwinMoveitAction("hiwin_6dot/follow_joint_trajectory");
        }
      
      double input[6]={ 0, 0, 0, 60, 0, 0};
      ptp_rel_pos(device_id, 1, input);
      ROS_WARN("Failed Excution.\n");
    }
    Close(device_id);
     
  }

    void HiwinPannel::refresh()
  {
    double return_pos[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    HROBOT device_id = Connect("169.254.128.198", 1, callBack);
    get_current_position(device_id,return_pos);
    tab1_X = return_pos[0];
    tab1_Y = return_pos[1];
    tab1_Z = return_pos[2];
    tab1_A = return_pos[3];
    tab1_B = return_pos[4];
    tab1_C = return_pos[5];
    output_topic_editor_->setText(QString::fromStdString(std::to_string(tab1_X)));
    output_topic_editor_1->setText(QString::fromStdString(std::to_string(tab1_Y)));
    output_topic_editor_2->setText(QString::fromStdString(std::to_string(tab1_Z)));
    output_topic_editor_3->setText(QString::fromStdString(std::to_string(tab1_A)));
    output_topic_editor_4->setText(QString::fromStdString(std::to_string(tab1_B)));
    output_topic_editor_5->setText(QString::fromStdString(std::to_string(tab1_C)));
  
  }

    void HiwinPannel::home()
  {
    HROBOT device_id = Connect("169.254.128.198", 1, callBack);
    if (device_id < 0)
    {
        
        ROS_WARN("Failed Connection.\n");
    }
    else
    {
      set_override_ratio(device_id, 100);

        if (get_motor_state(device_id) == 0)
        {
            set_motor_state(device_id, 1); // Servo on
                                           // HiwinMoveitAction HiwinMoveitAction("hiwin_6dot/follow_joint_trajectory");
        }
      
      double input[6]={0,0,0,0,-90,0};
      ptp_axis(device_id, 1, input);
      ROS_WARN("Failed Excution.\n");
    }
    Close(device_id);
  }


  /*
  void HiwinPannel::setTopic( const QString& new_topic )
  {
    if( new_topic != output_topic_ )
    {
      output_topic_ = new_topic;
      if( output_topic_ == "" )
      {
        velocity_publisher_.shutdown();
      }
      else
      {
        velocity_publisher_ = nh_.advertise<geometry_msgs::Twist>( output_topic_.toStdString(), 1 );
      }
      Q_EMIT configChanged();
    }
  }*/

  /*
  void HiwinPannel::sendVel()
  {
    if( ros::ok() && velocity_publisher_ )
    {
      geometry_msgs::Twist msg;
      msg.linear.x = 0;
      msg.linear.y = 0;
      msg.linear.z = 0;
      msg.angular.x = 0;
      msg.angular.y = 0;
      msg.angular.z = 1;
      velocity_publisher_.publish( msg );
    }
  }*/
  void HiwinPannel::save( rviz::Config config ) const
  {
    rviz::Panel::save( config );
    config.mapSetValue( "Topic", output_topic_ );
  }
  void HiwinPannel::load( const rviz::Config& config )
  {
    rviz::Panel::load( config );
    QString topic;
    if( config.mapGetString( "Topic", &topic ))
    {
      
    }
  }

}
#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(hiwin_rviz_plugin::HiwinPannel,rviz::Panel )