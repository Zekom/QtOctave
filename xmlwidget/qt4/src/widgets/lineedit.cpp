/* Copyright (C) 2007 P.L. Lucas
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, 
 * Boston, MA 02111-1307, USA. 
 */

#include "lineedit.h"

#include <QMessageBox>

Widget *new_lineedit(const QString & qName, const QXmlAttributes & atts, QWidget *parent)
{
	if(qName != "lineedit") return NULL;
	
	LineEdit *w=new LineEdit();
	QString id=atts.value("id");
	if(!id.isEmpty()) w->name=id;
	w->container=false;
	w->widget=new mLineEdit(parent);
	w->type=LINEEDIT;
	//(( mLineEdit*)(w->widget))->setWidget((Widget*)w);
	return (Widget *)w;
}

LineEdit::LineEdit():Widget()
{
	text_property_ok=false;
	startmark=linestartmark=endmark="";
}

bool LineEdit::xml_start(const QString & qName, const QXmlAttributes & atts)
{
	//printf("Procesando botón %s\n",qName.toLocal8Bit().data() );
	//mLineEdit *w=(mLineEdit *)widget;
	if(qName=="lineedit" && check_id(atts) )
	{
		//QString icon=atts.value("icon");
		//if(!title.isEmpty()) widget->setWindowTitle ( atts.value("title") );
		
		
		widget_properties(atts,this);
		
		QString password=atts.value("password");
		
		if(password=="true")
		{
			mLineEdit *l=(mLineEdit *)widget;
			l->setEchoMode(QLineEdit::Password);
		}
		
		return true;
	}
	else if(qName=="text")
	{
		return true;
	}
	else if(qName=="get_text")
	{
		text_property_ok=true;
		return true;
	}
	else if(text_property_ok)
	{
		QString value=atts.value("value");
		if(!value.isEmpty())
		{
			if(qName=="startmark") startmark=value;
			else if(qName=="endmark") endmark=value;
			else if(qName=="linestartmark") linestartmark=value;
		}
		return true;
	}
	
	return false;
}

bool LineEdit::xml_end(const QString & qName, const QString &text, bool &property_ok)
{
	if(type!=LINEEDIT)
	{
		return false;
	}
	mLineEdit *l=(mLineEdit *)widget;
	if(qName=="lineedit")
	{
		property_ok=false;
		return true;
	}
	else if(qName=="text")
	{
		l->setText(text);
		property_ok=true;
		return true;
	}
	else if(qName=="get_text")
	{
		QString text=l->text();
		if(linestartmark.isEmpty() && startmark.isEmpty() && endmark.isEmpty() )
		{
			text.replace("<","&lt;");
			text.replace(">","&gt;");
			output->write("<lineedit id=\""+name+"\">\n<text>"+toXML(text)+"</text>\n</lineedit>\n");
		}
		else
		{
			output->write(startmark);
			output->write(linestartmark+text);
			output->write(endmark+"\n");
			startmark=linestartmark=endmark="";
		}
		text_property_ok=false;
		property_ok=true;
		return true;
	}
	else if(text_property_ok)
	{
		if(qName=="startmark" || qName=="endmark" || qName=="linestartmark")
		{
			property_ok=true;
			return true;
		}
	}
	
	
	return false;
}


void LineEdit::add_widget(Widget */*w*/)
{
	;
}



mLineEdit::mLineEdit( QWidget * parent):QLineEdit(parent)
{
}




