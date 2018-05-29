#include "tetris.h"
#include <app.h>
#include <Elementary.h>
#include <efl_extension.h>
#include <dlog.h>
#include <stdlib.h>

typedef struct _UIData {
   Evas_Object *win;
   Evas_Object *conform;
   Evas_Object *grid;
   Evas_Object *rect_table; // ��Ʈ���� ���� ����� ���� ���̺�_�簢������ ��ġ�Ѵ�
   Evas_Object *next_block_table; // ���� ����� �����ֱ� ���� ���̺� _ ���� ��� ��ġ
   Evas_Object *score_label; // ������ ǥ���ϴ� ���̺�
   Evas_Object *rect[240]; // ��Ʈ���� ���� �̷�� �簢����
   Evas_Object *next_rect[12]; // next block�� �����ִ� �簢����
   Evas_Object *popup; // �˾�â�� ���� ���� ������Ʈ
   Evas_Object *popup_box; // �˾�â ������ ������� ��ġ
   Evas_Object *data;
   Evas_Object *nf;
   int timer_count;
} UIData;

Ecore_Timer *timer1; // 1�ʸ��� �����ϴ� Ÿ�̸�
int pos; // ����� �׸��� ������ �Ǵ� ��
int shape=0; // ����� ����� �ٲٴ� ����
int rotation=0; // ����� ������ �ٲٴ� ����
int a,b,c,d; // ����� �� �簢���� ����Ű�� ������
int wall[240]={0,}; // ��Ͽ� ���� �ִ��� �������� ǥ���Ѵ�. 0�̸� false, 1�̸� true
int next=0; // ���� ����� ����� �����ϴ� ����
int buttondrop=0; // ��ư�� ���ȴ����� Ȯ���ϴ� ����
int stop_button=0;
int score=0; // ������ �����ϴ� ��ư
bool pos_flag=true; // position�� �ٲ������ ǥ���ϴ� ����

static void
_window_create(UIData *ui);
static void
_grid_create(UIData *ui);
static void
_tetris_delete(UIData *ui, int i);

static void
_win_back_cb(void *data, Evas_Object *obj, void *event_info)
{
	ui_app_exit();
}

static bool
_check_flag() { // a,b,c,d�� ��� ���� �ƴϸ� true�� ����
   bool flag=true;
   int i=0;
   for(i=0;i<4;i++)
      if(wall[a]==1||wall[b]==1||wall[c]==1||wall[d]==1) flag=false;
   return flag;
}

static void
_change_rotation() { // ����� ������ ����, ������ �������ִ� �Լ�
   if(shape==0||shape==3||shape==5) {
      if(rotation==0) rotation++;
      else rotation=0;
   }
   else if(shape==1||shape==2||shape==4) {
      if(rotation<3) rotation++;
      else rotation=0;
   }
}

static void
_make_shape() { // ����� ���, ���⿡ ���� a,b,c,d �� �������� �����ش�

   switch(shape) {
   case(0) :
      if(rotation==0) {
         /*
          * ��
          * ���
          *  ��
          */
         a=pos;
         b=pos+12;
         c=pos+25;
         d=pos+13;
      } else {
         /*
          *  ���
          * ���
          *
          * */
         a=pos+1;
         b=pos+12;
         c=pos+13;
         d=pos+2;
      }
      break;
   case(1):
      if(rotation==0) {
         /*
          * ���
          * ��
          * ��
          * */
         a=pos;
         b=pos+12;
         c=pos+24;
         d=pos+1;
      } else if(rotation==1) {
         /*
          * ��
          * ����
          * */
         a=pos;
         b=pos+12;
         c=pos+13;
         d=pos+14;
      } else if(rotation==2) {
         /*
          *  ��
          *  ��
          * ���
          * */
         a=pos+1;
         b=pos+24;
         c=pos+25;
         d=pos+13;
      } else if(rotation==3) {
         /*
          * ����
          *   ��
          *
          * */
         a=pos+1;
         b=pos;
         c=pos+14;
         d=pos+2;
      }
      break;
   case(2):
      if(rotation==0) {
         /*
          *  ���
          *   ��
          *   ��
          * */
         a=pos+1;
         b=pos;
         c=pos+25;
         d=pos+13;
      } else if(rotation==1) {
         /*
          * ����
          * ��
          * */
         a=pos+1;
         b=pos;
         c=pos+12;
         d=pos+2;
      } else if(rotation==2) {
         /*
          * ��
          * ��
          * ���
          * */
         a=pos;
         b=pos+12;
         c=pos+24;
         d=pos+25;
      } else if(rotation==3) {
         /*
          *   ��
          * ����
          * */
         a=pos+2;
         b=pos+12;
         c=pos+13;
         d=pos+14;
      }
      break;
   case(3):
      if(rotation==0) {
         /*
          *  ��
          * ���
          * ��
          * */
         a=pos+1;
         b=pos+12;
         c=pos+24;
         d=pos+13;
      } else if(rotation==1) {
         /*
          * ���
          *  ���
          *
          * */
         a=pos+1;
         b=pos;
         c=pos+13;
         d=pos+14;
      }
      break;
   case(4):
      if(rotation==0) {
         /*
          *  ��
          * ����
          * */

         a=pos+1;
         b=pos+12;
         c=pos+13;
         d=pos+14;
      } else if(rotation==1) {
         /*  ��
          * ���
          *  ��
          * */
         a=pos+1;
         b=pos+12;
         c=pos+13;
         d=pos+25;
      } else if(rotation==2) {
         /*
          *  ����
          *   ��
          * */
         a=pos+13;
         b=pos+12;
         c=pos+14;
         d=pos+25;
      } else if(rotation==3) {
         /*
          *  ��
          *  ���
          *  ��
          * */
         a=pos+1;
         b=pos+13;
         c=pos+25;
         d=pos+14;
      }
      break;
   case(5):
      if(rotation==0) {
         /*
          *  ��
          *  ��
          *  ��
          *  ��
          * */
         a=pos+1;
         b=pos+13;
         c=pos+37;
         d=pos+25;
      } else if(rotation==1) {
         /*
          *  �����
          *
          * */
         a=pos+1;
         b=pos;
         c=pos+2;
         d=pos+3;
      }
      break;
   case(6):
         /*
          * ���
          * ���
          * */
      a=pos+1;
      b=pos;
      c=pos+12;
      d=pos+13;
      break;
   }
}

static void
_draw_block(UIData *ui){ // �׷����� �� �ڸ��� ���� ������, ����� �׷��ش�.

   int prev_a=a;
   int prev_b=b;
   int prev_c=c;
   int prev_d=d;
   int prev_rotation = rotation; // ���� �׸� �� ������ ������ ����� �ٽ� �׷����Ѵ�.

   _make_shape();
   if(_check_flag()) {
      evas_object_color_set(ui->rect[a],100,100,100,255);
      evas_object_color_set(ui->rect[b],100,100,100,255);
      evas_object_color_set(ui->rect[c],100,100,100,255);
      evas_object_color_set(ui->rect[d],100,100,100,255);
      pos_flag= true;
   } else {
      a=prev_a;
      b=prev_b;
      c=prev_c;
      d=prev_d;
      rotation = prev_rotation;
      evas_object_color_set(ui->rect[a],100,100,100,255);
      evas_object_color_set(ui->rect[b],100,100,100,255);
      evas_object_color_set(ui->rect[c],100,100,100,255);
      evas_object_color_set(ui->rect[d],100,100,100,255);
      pos_flag=false;
   }

}

static void
_remove_block(UIData *ui){ // ����� �ű�� ���� �����.

   evas_object_color_set(ui->rect[a],246,246,246,255);
   evas_object_color_set(ui->rect[b],246,246,246,255);
   evas_object_color_set(ui->rect[c],246,246,246,255);
   evas_object_color_set(ui->rect[d],246,246,246,255);

}

//��� ����
static void
_save_block(UIData *ui) {
   int i=0,j=0;
   char buf[64]; // ������ �Է��ϱ� ���� buffer
   bool flag=true; // ���� ä�����ֳ��� �˻��ϴ� �÷���

   // ���� ��ġ�� ����� ����
   wall[a]=1;
   wall[b]=1;
   wall[c]=1;
   wall[d]=1;

   for(i=1;i<19;i++) {
      flag=true;
      for(j=1;j<11;j++)
         if(wall[i*12+j]==0) flag=false;
      if(flag) // ��� �簢���� ���ִٸ� �� ���� ������ ���� �׸��� ������ �ø���.
      {
        _tetris_delete(ui,i);
        score++;
        sprintf(buf,"<font=Sans:style=Regular font_size=50>Score : %d</font/>",score);
        elm_object_text_set(ui->score_label, buf);
      }
   }
}

static void
next_block_draw(UIData *ui) { // ���� ����� �׷��ִ� �Լ� - ���� ���
   int temp[4]; // next block�� a,b,c,d ������ �Ѵ�.
   int i,j;
   for(i=0;i<12;i++) // ���� ����� �����
      evas_object_color_set(ui->next_rect[i],246,246,246,255);

   switch(next) { // ��翡 ���� temp ���� �����Ѵ�.
            case(0) :
            temp[0]=0;
            temp[1]=3;
            temp[2]=4;
            temp[3]=7;
               break;
            case(1):
            temp[0]=0;
            temp[1]=1;
            temp[2]=3;
            temp[3]=6;
               break;
            case(2):
            temp[0]=0;
            temp[1]=1;
            temp[2]=4;
            temp[3]=7;
               break;
            case(3):
            temp[0]=2;
            temp[1]=4;
            temp[2]=5;
            temp[3]=7;
               break;
            case(4):
                 temp[0]=4;
            temp[1]=6;
            temp[2]=7;
            temp[3]=8;
            break;
            case(5):
            temp[0]=1;
            temp[1]=4;
            temp[2]=7;
            temp[3]=10;
               break;
            case(6):
            temp[0]=6;
            temp[1]=7;
            temp[2]=9;
            temp[3]=10;
               break;
            }

         for(i=0;i<4;i++) { // ���̺� �׷��ش�.
            j=temp[i];
            evas_object_color_set(ui->next_rect[j],100,100,100,255);
         }
}

//��� ������ �� �ٽ� �׸���(��ĥ)
static void
_draw_after(UIData *ui){
   int i=0;
   while(i<240){
      //�׵θ�
      if(i%12==0||i%12==11||i/12==0||i/12==19){
         evas_object_color_set(ui->rect[i],6,162,203,255);
      }
      //��� ����
      else if(wall[i]==1){
         evas_object_color_set(ui->rect[i],100,100,100, 255);
      }
      //��� ���� �� �ϸ� ����
      else{
         evas_object_color_set(ui->rect[i], 246,246,246, 255);
      }
      i++;
   }
}

//��� �� �� �� ä������ ����(wall[])
static void
_tetris_delete(UIData *ui, int i){
   int k;
   for(k=1;k<11;k++) wall[i*12+k]=0; // i��° ���� �� �� ���̹Ƿ�, �����ش� - ���̻� wall�� �ƴϴ�

   //������ �� ���� �ִ� ���� ������ �� �پ� ������
   while(i>1) {
      for(k=1;k<11;k++) {
        wall[i*12+k]=wall[(i-1)*12+k];
        wall[(i-1)*12+k]=0;
      }
      i--;
   }
   _draw_after(ui);
}

// P334~ Touch event
/* Touch �ٿ� �̺�Ʈ*/

static void
on_mouse_down(void *data, Evas *e, Evas_Object *obj, void *event_info) {

   UIData *ui=data;
   int temp_pos=pos;
   int temp_rot=rotation;
      if(stop_button!=1){
      // temppos : 0~270��, ����ڰ� ���� rect��ü�� ID.
      int temppos = atoi(evas_object_name_get(obj));

      // x, y : ����ڰ� ���� Rect�� ��ǥ��
      int x = temppos%12;
      int y = temppos/12;

      _remove_block(ui);
      // ����� �Ʒ��� ������ ������ �Ʒ��� ���� �̵��Ѵ�.
      if((y-2>(pos/12))&&((x==a%12)||(x==b%12)||(x==c%12)||(x==d%12))){

         // �� �� �̵�
         pos+=12;
         _draw_block(ui);
         if(pos_flag) {
           pos+=12;
           _remove_block(ui);
           _draw_block(ui);
      }
      else pos-=12;
      }
      // ����� ������ ��� ����� �ٲ۴�.
      else if(temppos==a||temppos==b||temppos==c||temppos==d){
         _change_rotation();
      }

      // ����� ���� ������ ������ �������� �̵��Ѵ�.
      else if(x<=(pos%12)){
         pos--;
      }

      // ����� ������ ������ ������ ���������� �̵��Ѵ�.
      else if(x>(pos%12)){
         pos++;
      }

      _draw_block(ui);
      if(!pos_flag) {
         pos=temp_pos;
         rotation=temp_rot;
      }

   }
}

static void
btn_change_cb(void *data, Evas_Object *obj, void *event_info){
   int temp_rot=rotation;
   if(stop_button!=1){
      UIData *ui = data;
       elm_object_content_set(ui->conform, ui->grid);
         _remove_block(ui);
         _change_rotation();
         _draw_block(ui);
         if(!pos_flag) rotation=temp_rot;
   }
}

static void
stop_button_event(){
   if(stop_button==1){
      stop_button=0;
   }
   else{
    stop_button=1;
   }
}


//stop_button click
static void
continue_button_click(void *data,Evas_Object *obj, void *event_info){
   UIData *ui = data;
   stop_button_event();
   evas_object_del(ui->popup); //�˾�â ����

}

static void
btn_reset_cb(void *data, Evas_Object *obj, void *event_info){
   stop_button_event();

   UIData *ui = data;
   _grid_create(ui);
   elm_object_content_set(ui->conform, ui->grid);
   score=0;

   evas_object_del(obj); //�˾�â ����
}

static void
terminate_button_click(void *data, Evas_Object *obj, void *event_info){
    UIData *ui = data;
    ui=0;
    evas_object_del(ui->win);
}

static void
btn_drop_cb(void *data, Evas_Object *obj, void *event_info){
    if(stop_button!=1){
       UIData *ui = data;
       elm_object_content_set(ui->conform, ui->grid);
       buttondrop=1;
    }
}

// ������ ����Ǿ��� ��� �ߴ� �˾�
static void
make_popup_end(UIData *u){
   Evas_Object *btn;
   UIData *ui = u;

   //�켱 ����� ����
   stop_button_event();

   //popup ����
   ui->popup=elm_popup_add(ui->grid); //�˾� ����
   elm_popup_align_set(ui->popup,ELM_NOTIFY_ALIGN_FILL,1.0); //��ġ ����
   evas_object_size_hint_weight_set(ui->popup,EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   //�ٽ��ϱ� ��ư ����
   btn=elm_button_add(ui->popup);
   elm_object_text_set(btn,"�ٽ��ϱ�");
   elm_object_part_content_set(ui->popup,"button1",btn);
   evas_object_smart_callback_add(btn,"clicked",btn_reset_cb,ui);
   evas_object_show(btn);

   //�׸��ϱ� ��ư ����
   btn=elm_button_add(ui->popup);
   elm_object_text_set(btn,"�׸��ϱ�");
   elm_object_part_content_set(ui->popup,"button2",btn);
   evas_object_smart_callback_add(btn,"clicked",terminate_button_click,ui);
   evas_object_show(btn);

   evas_object_show(ui->popup);
}

// �޴����� POPUP ��ư Ŭ���� ��� �̺�Ʈ
static void
make_popup_button(void *data, Evas_Object *obj, void *event_info){
   Evas_Object *btn;
   UIData *ui = data;

   //�켱 ����� ����
   stop_button_event();

   //popup ����
   ui->popup=elm_popup_add(ui->grid); //�˾� ����
   elm_popup_align_set(ui->popup,ELM_NOTIFY_ALIGN_FILL,1.0); //��ġ ����
   evas_object_size_hint_weight_set(ui->popup,EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   //����ϱ� ��ư ����
   btn=elm_button_add(ui->popup);
   elm_object_text_set(btn,"����ϱ�");
   elm_object_part_content_set(ui->popup,"button1",btn);
   evas_object_smart_callback_add(btn,"clicked",continue_button_click,ui);
   evas_object_show(btn);

   //�ٽ��ϱ� ��ư ����
   btn=elm_button_add(ui->popup);
   elm_object_text_set(btn,"�ٽ��ϱ�");
   elm_object_part_content_set(ui->popup,"button2",btn);
   evas_object_smart_callback_add(btn,"clicked",btn_reset_cb,ui);
   evas_object_show(btn);

   //�׸��ϱ� ��ư ����
   btn=elm_button_add(ui->popup);
   elm_object_text_set(btn,"�׸��ϱ�");
   elm_object_part_content_set(ui->popup,"button3",btn);
   evas_object_smart_callback_add(btn,"clicked",terminate_button_click,ui);
   evas_object_show(btn);

   evas_object_show(ui->popup);
}

static void
_init_draw_block(UIData *ui, int i){ // ó���� ��Ʈ���� ���� wall ���� �ʱ�ȭ�ϰ� ù ����� �׸��� �Լ�
   int j=0;
   pos = i;
   _draw_block(ui);

   for(j=0;j<240;j++) {
       if (j%12==0) wall[j]=1;
       else if(j%12==11) wall[j]=1;
       else if(j>227) wall[j]=1;
       else wall[j]=0;
   }
}

static void
_rect_table_create(UIData *ui) {
   int i=0;
   int x=0,y=0;

   //Rect ���̺� ����
   ui->rect_table = elm_table_add(ui->grid);

   // rect_table : 12x20
   for(i=0;i<240;i++) {

     ui->rect[i]=evas_object_rectangle_add(ui->rect_table);
     evas_object_color_set(ui->rect[i], 246,246,246, 255);

      // �׵θ�
     if(i%12==0||i%12==11||i/12==0||i/12==19)
         evas_object_color_set(ui->rect[i],6,162,203,255);

     evas_object_size_hint_min_set(ui->rect[i], 50, 50);

      y=i/12;
      x=i%12;

     elm_table_padding_set(ui->rect_table,5,5);
     elm_table_pack(ui->rect_table, ui->rect[i],x,y,1,1);

     // Ŭ���� �簢���� x��ǥ ���
     char *num[3];
     sprintf(num, "%d", i);
     evas_object_name_set(ui->rect[i], num);

     //�簢���� �ݹ��Լ�
     evas_object_event_callback_add(ui->rect[i],   EVAS_CALLBACK_MOUSE_DOWN, on_mouse_down, ui);

     evas_object_show(ui->rect[i]);
   }

}

static void
_next_rect_table_create(UIData *ui) {
   int i=0;
   int x,y;
   ui->next_block_table = elm_table_add(ui->grid);

   for(i=0;i<12;i++) { // next_rect[] ��ĥ
      ui->next_rect[i]=evas_object_rectangle_add(ui->next_block_table);
      evas_object_color_set(ui->next_rect[i],246,246,246, 255);
      evas_object_size_hint_min_set(ui->next_rect[i],25,25);

      y=i/3;
      x=i%3;

      // next_block_table�� �簢�� ��ŷ
      elm_table_padding_set(ui->next_block_table, 5,5);
      elm_table_pack(ui->next_block_table,ui->next_rect[i],x,y,1,1);
      evas_object_show(ui->next_rect[i]);
   }
}
static void
_grid_create(UIData *ui)
{

   Evas_Object *btn;

   //Grid ����
   ui->grid = elm_grid_add(ui->conform);
   evas_object_show(ui->grid);

   //change button ���
   btn = elm_button_add(ui->grid);
   elm_object_text_set(btn, "Change");
   evas_object_smart_callback_add(btn,"clicked",btn_change_cb,ui);
   elm_grid_pack(ui->grid,btn,3,1,25,3);
   evas_object_show(btn);

   //drop button ���
   btn = elm_button_add(ui->grid);
   elm_object_text_set(btn, "Drop");
   evas_object_smart_callback_add(btn,"clicked",btn_drop_cb,ui);
   elm_grid_pack(ui->grid,btn,60,1,18,3);
   evas_object_show(btn);

   //�˾�â ��ư
   btn=elm_button_add(ui->conform);
   elm_object_text_set(btn,"POPUP");
   evas_object_smart_callback_add(btn,"clicked",make_popup_button, ui);
   elm_grid_pack(ui->grid,btn,32,1,24,3);
   evas_object_show(btn);

   // ���� �� ���
   char buf[64];
   ui->score_label=elm_label_add(ui->grid);
   sprintf(buf,"<font=Sans:style=Regular font_size=50>Score : %d</font/>",score);
   elm_object_text_set(ui->score_label, buf);
   elm_grid_pack(ui->grid,ui->score_label,10,5,30,10);
   evas_object_show(ui->score_label);

   // rect_table ����
   _rect_table_create(ui);

   /* wall[] �� �ʱ�ȭ & ���� ��� ����*/
   _init_draw_block(ui,17);

   // next block table ���
   _next_rect_table_create(ui);

   // ���� ��� �� ����
   next=rand()%7;
   next_block_draw(ui);

   // grid �� �� ���̺� ��ŷ
   elm_grid_pack(ui->grid,ui->rect_table,5,15,90,80);
   elm_grid_pack(ui->grid,ui->next_block_table, 85,1,10,10);
   evas_object_show(ui->rect_table);
   evas_object_show(ui->next_block_table);

}

Eina_Bool timer_func(void *data) { // 1�ʸ��� ����� ��������.
   UIData *ui=data;
   if(stop_button!=1){
      if(buttondrop==1){ // drop button�� �������
          while(pos_flag) {
             _remove_block(data);
            pos+=12;
           _draw_block(data);
           if(!pos_flag) pos-=12;
          }
          buttondrop=0;
      }

   if(wall[a+12]==1||wall[b+12]==1||wall[c+12]==1||wall[d+12]==1) {
      // ����� �ؿ� ������ ���߾� �����Ѵ�.
      _save_block(ui);
      pos=17;
      shape=next;
      next=rand()%7;
      // next block�� �ٲ���
      next_block_draw(data);
      rotation=0;
      _draw_block(data);
      if(!pos_flag) make_popup_end(ui);
         } else { // �� ������ ���� ������ ����� �� ĭ ������.
        _remove_block(ui);
        pos+=12;
        _draw_block(ui);
         }
   }
   return ECORE_CALLBACK_RENEW;
}

static void
_conformant_create(UIData *ui)
{
   ui->conform = elm_conformant_add(ui->win);

   //Specify to conformant occupies all allocated to him space
  // (horizontally and vertically)
   evas_object_size_hint_weight_set(ui->conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   // Create a layout and place it into a conformant
   _grid_create(ui);
   elm_object_content_set(ui->conform, ui->grid);

   evas_object_show(ui->conform);
}
static void
_window_create(UIData *ui)
{
   // Create and configure the main window
   ui->win = elm_win_util_standard_add(NULL, NULL);

       // Create a conformant - in this case, the main container in the application,
       // which is also the interface between the application and
       // system elements, such as a keyboard.
   _conformant_create(ui);

   //  Set the size of the conformant widget the same as the size of the window
   elm_win_resize_object_add(ui->win, ui->conform);

    //Register the function that will be called
    //when you press the hardware Back button
   eext_object_event_callback_add(ui->win, EEXT_CALLBACK_BACK, _win_back_cb, ui);

   // /Always display the window only after the entire base
   // user interface will be displayed.
   evas_object_show(ui->win);
}

static bool
_app_create_cb(void *data)
{
   UIData *ui = data;

   _window_create(ui);
   timer1=ecore_timer_add(1,timer_func,ui);
   return true;
}

/*
static void
_app_terminate_cb(void *data)
{
   UIData *ui = data;

   //Before closing the application, delete the main widget (window),
    // then all  "children" widgets of this window (all widgets of this application) will be deleted.
   evas_object_del(ui->win);
}
*/


void
create_game_start_cb(UIData *ui)
{
	_window_create(ui);
	_app_create_cb(ui);

}
