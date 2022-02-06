/*
  EZPMainConsole.cpp

  Controls the GEMBIRD Silver Shield PM USB outlet device

  (C) 2006, Mondrian Nuessle, Computer Architecture Group, University of Mannheim, Germany

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


  nuessle@uni-mannheim.de

*/

// modified by YoungTaek Oh.


// Further modifed by Emre Karagoz

// TODO: Multiple plots in a window
// TODO: Major rewrite

#ifdef WIN32
#   undef _DEBUG
#endif
//#include <cstdio>

#include "Python.h"
#include "QtCore\qdebug.h"

#include "Common.h"
#include "Math\Math.h"
#include "EZPMainConsole.h"
#include "Renderer\Plot2DRenderer.h"
#include "Renderer\Plot3DRenderer.h"

PyObject* glb;
PyObject* loc;

QString resultString;

static PyObject* redirector_init(PyObject *, PyObject *)
{
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* redirector_write(PyObject *, PyObject *args)
{
	char* output;
	PyObject *selfi;
	
	if (!PyArg_ParseTuple(args,"Os",&selfi,&output))
	{
		return NULL;
	}

	QString outputString = QString::fromLocal8Bit(output);
	resultString.append(outputString);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyMethodDef redirectorMethods[] =
{
	{"__init__", redirector_init, METH_VARARGS,
	 "initialize the stdout/err redirector"},
	{"write", redirector_write, METH_VARARGS,
	 "implement the write method to redirect stdout/err"},
	{NULL,NULL,0,NULL},
};


static PyObject* EZP_Plot2D(PyObject* Self, PyObject* Args)
{	
	PyObject* Arg0; // GraphVertexData
	U32 Arg1; // GraphVertexCount
	F32 Arg2; // XMin
	F32 Arg3; // XMax
	F32 Arg4; // YMin
	F32 Arg5; // YMax
	U32 Arg6; // PlotID
	PyArg_ParseTuple(Args, "OIffffI", &Arg0, &Arg1, &Arg2, &Arg3, &Arg4, &Arg5, &Arg6);

	std::vector<Math::Vector3> GraphVertexArray;
	GraphVertexArray.reserve(sizeof(Math::Vector3) * Arg1);

	for (U32 Idx = 0; Idx < Arg1 * 2; Idx += 2)
	{
		Math::Vector3 VertexPos;
		VertexPos.x = static_cast<F32>(PyFloat_AsDouble(PyTuple_GetItem(Arg0, Idx)));
		VertexPos.y = static_cast<F32>(PyFloat_AsDouble(PyTuple_GetItem(Arg0, Idx + 1)));
		VertexPos.z = 0.0f;

		GraphVertexArray.push_back(VertexPos);
	}	

	U32 PlotID = Arg6 - 1;

	EZPRenderWidgetD3D10** ppRenderWidget = &(EZPMainConsole::getInstance()->RenderWidgetArray[PlotID]);
	
	if ((*ppRenderWidget) == nullptr)
	{
		(*ppRenderWidget) = new EZPRenderWidgetD3D10();
		(*ppRenderWidget)->SetPlotID(PlotID);
		(*ppRenderWidget)->setWindowTitle(QString("Plot ") + QString::number(Arg6));
		(*ppRenderWidget)->show();
	}

	(*ppRenderWidget)->SetPlottingMode(EPlottingMode::PLOT_2D);
	
	Plot2DRenderData RenderData;
	RenderData.GraphVertexCount = Arg1;
	RenderData.LineColor = { 1.0f, 0.0f, 0.0f, 1.0f };
	RenderData.pGraphVertexArray = &GraphVertexArray;
	RenderData.XMin = Arg2;
	RenderData.XMax = Arg3;
	RenderData.YMin = Arg4;
	RenderData.YMax = Arg5;

	Plot2DRenderState RenderState;
	RenderState.pRenderTarget = (*ppRenderWidget)->GetBackBufferPtr();

	Plot2DRenderer::Initialize((*ppRenderWidget)->GetRenderSystemInterfacePtr(), RenderData, RenderState);
	Plot2DRenderer::Render((*ppRenderWidget)->GetRenderSystemInterfacePtr(), RenderState);

	(*ppRenderWidget)->update();

	return Py_None;
}   

static PyObject* EZP_Plot3D(PyObject* Self, PyObject* Args)
{

	PyObject* Arg0; // GraphVertexArray
	U32 Arg1; // GridDim
	F32 Arg2; // XMin
	F32 Arg3; // XMax
	F32 Arg4; // YMin
	F32 Arg5; // YMax
	F32 Arg6; // ZMin
	F32 Arg7; // ZMax
	U32 Arg8; // PlotID

	PyArg_ParseTuple(Args, "OIffffffI", &Arg0, &Arg1, &Arg2, &Arg3, &Arg4, &Arg5, &Arg6, &Arg7, &Arg8);

	// [64][64*3] array
	std::vector<Math::Vector3> GraphVertexArray;
	GraphVertexArray.reserve(Arg1 * Arg1);

	for (U32 Idx = 0; Idx < Arg1 * Arg1 * 3; Idx += 3)
	{
		Math::Vector3 VertexPos;
		VertexPos.x = static_cast<F32>(PyFloat_AsDouble(PyTuple_GetItem(Arg0, Idx)));
		VertexPos.y = static_cast<F32>(PyFloat_AsDouble(PyTuple_GetItem(Arg0, Idx + 1)));
		VertexPos.z = static_cast<F32>(PyFloat_AsDouble(PyTuple_GetItem(Arg0, Idx + 2)));

		GraphVertexArray.push_back(VertexPos);
	}

	U32 PlotID = Arg8 - 1;

	EZPRenderWidgetD3D10** ppRenderWidget = &(EZPMainConsole::getInstance()->RenderWidgetArray[PlotID]);

	if ((*ppRenderWidget) == nullptr)
	{
		(*ppRenderWidget) = new EZPRenderWidgetD3D10();
		(*ppRenderWidget)->SetPlotID(PlotID);
		(*ppRenderWidget)->setWindowTitle(QString("Plot ") + QString::number(Arg8));
		(*ppRenderWidget)->show();
	}
	
	(*ppRenderWidget)->SetPlottingMode(EPlottingMode::PLOT_3D);

	Plot3DRenderData* pRenderData = new Plot3DRenderData;
	pRenderData->pGraphVertexArray = &GraphVertexArray;
	pRenderData->GraphVertexCount = Arg1 * Arg1;
	pRenderData->XMax = Arg2;
	pRenderData->XMin = Arg3;
	pRenderData->YMax = Arg4;
	pRenderData->YMin = Arg5;
	pRenderData->ZMax = Arg6;
	pRenderData->ZMin = Arg7;

	Plot3DRenderState* pRenderState = new Plot3DRenderState;
	pRenderState->pRenderTarget = (*ppRenderWidget)->GetBackBufferPtr();

	Plot3DRenderer::Initialize((*ppRenderWidget)->GetRenderSystemInterfacePtr(), pRenderData, pRenderState);
	Plot3DRenderer::Render((*ppRenderWidget)->GetRenderSystemInterfacePtr(), pRenderState);

	(*ppRenderWidget)->SetPlot3DRenderData(pRenderData);
	(*ppRenderWidget)->SetPlot3DRenderState(pRenderState);

	(*ppRenderWidget)->update();

	return Py_None;
}

static PyObject* EZP_SavePlot(PyObject* Self, PyObject* Args)
{
	const char* Arg0; // FileName
	const char* Arg1; // ImageFormat
	U32	Arg2; // PlotID

	PyArg_ParseTuple(Args, "ssI", &Arg0, &Arg1, &Arg2);
	
	const char* FileName = Arg0;
	const char* ImageFormat = Arg1;
	U32 PlotID = Arg2 - 1;

	EZPMainConsole::getInstance()->RenderWidgetArray[PlotID]->SaveFramebufferAsImage(FileName, ImageFormat);

	return Py_None;
}

static PyObject* py_clear(PyObject *, PyObject *)
{
	EZPMainConsole::getInstance()->clear();
	return Py_None;
}

static PyObject* py_reset(PyObject *, PyObject *)
{
	EZPMainConsole::getInstance()->reset();
	return Py_None;
}

static PyObject* py_save(PyObject *, PyObject *args)
{
	char* filename;
	if (!PyArg_ParseTuple(args,"s",&filename))
	{
		return NULL;
	}
	EZPMainConsole::getInstance()->saveScript(filename);
	return Py_None;
}

static PyObject* py_load(PyObject *, PyObject *args)
{
	char* filename;
	if (!PyArg_ParseTuple(args,"s",&filename))
	{
		return NULL;
	}
	EZPMainConsole::getInstance()->loadScript(filename);

	return Py_None;
}

static PyObject* py_history(PyObject *, PyObject *)
{
	EZPMainConsole::getInstance()->printHistory();
	return Py_None;
}

static PyObject* py_quit(PyObject *, PyObject *)
{
	resultString="Use reset() to restart the interpreter; otherwise exit your application\n";
	return Py_None;
}

static PyMethodDef ModuleMethods[] = { { NULL, NULL, 0, NULL } };
static PyMethodDef console_methods[] =  
{
	{ "clear",py_clear, METH_VARARGS,"clears the console" },
	{ "reset",py_reset, METH_VARARGS,"reset the interpreter and clear the console" },
	{ "save",py_save, METH_VARARGS,"save commands up to now in given file" },
	{ "load",py_load, METH_VARARGS,"load commands from given file" },
	{ "history",py_history, METH_VARARGS,"shows the history" },
	{ "quit",py_quit, METH_VARARGS,"print information about quitting" },
	{ "ezp_plot2D", EZP_Plot2D, METH_VARARGS, "plot 2-Dimensional functions" },
	{ "ezp_plot3D", EZP_Plot3D, METH_VARARGS, "" },
	{ "ezp_save_plot", EZP_SavePlot, METH_VARARGS, "" },
	{ NULL, NULL, 0, NULL }
};

void initredirector()
{
	PyMethodDef *def;

	/* create a new module and class */
	PyObject *module = Py_InitModule("redirector", ModuleMethods);
	PyObject *moduleDict = PyModule_GetDict(module);
	PyObject *classDict = PyDict_New();
	PyObject *className = PyString_FromString("redirector");
	PyObject *fooClass = PyClass_New(NULL, classDict, className);
	PyDict_SetItemString(moduleDict, "redirector", fooClass);
	Py_DECREF(classDict);
	Py_DECREF(className);
	Py_DECREF(fooClass);

	/* add methods to class */
	for (def = redirectorMethods; def->ml_name != NULL; def++) {
		PyObject *func = PyCFunction_New(def, NULL);
		PyObject *method = PyMethod_New(func, NULL, fooClass);
		PyDict_SetItemString(classDict, def->ml_name, method);
		Py_DECREF(func);
		Py_DECREF(method);
	}
}

void EZPMainConsole::printHistory()
{
	uint index = 1;
	for ( QStringList::Iterator it = history.begin(); it != history.end(); ++it )
	{
		resultString.append(QString("%1\t%2\n").arg(index).arg(*it));
		index ++;
	}
}

EZPMainConsole *EZPMainConsole::theInstance = NULL;

EZPMainConsole *EZPMainConsole::getInstance(QWidget *parent, const QString& welcomeText)
{
	if (!theInstance)
	{
		theInstance = new EZPMainConsole(parent, welcomeText);
	}
	return theInstance;
}

//QTcl console constructor (init the QTextEdit & the attributes)
EZPMainConsole::EZPMainConsole(QWidget *parent, const QString& welcomeText) :
		QConsole(parent, welcomeText),lines(0)
{
	parent->setAttribute(Qt::WA_DeleteOnClose, true);
	
	//set the Python Prompt
	setNormalPrompt(true);

	pRenderWidget = nullptr;
	Py_Initialize();
	/* NOTE: In previous implementaion, local name and global name
			 were allocated separately.  And it causes a problem that
			 a function declared in this console cannot be called.  By
			 unifying global and local name with __main__.__dict__, we
			 can get more natural python console.
	*/
	PyObject *module = PyImport_ImportModule("__main__");
	loc = glb = PyModule_GetDict(module);

	initredirector();

	PyImport_AddModule("console");
	Py_InitModule("console", console_methods);

	PyImport_ImportModule("rlcompleter");
	PyRun_SimpleString
	(
		"import sys\n"
		"from math import *\n"
		"import redirector\n"
		"import console\n"
		"import rlcompleter\n"
		"sys.path.insert(0, \".\")\n" // add current
									 // path
		"sys.stdout = redirector.redirector()\n"
		"sys.stderr = sys.stdout\n"
		"import __builtin__\n"
		"__builtin__.clear=console.clear\n"
		"__builtin__.reset=console.reset\n"
		"__builtin__.save=console.save\n"
		"__builtin__.load=console.load\n"
		"__builtin__.history=console.history\n"
		"__builtin__.quit=console.quit\n"
		"__builtin__.completer=rlcompleter.Completer()\n"
		"__builtin__.ezp_plot2D=console.ezp_plot2D\n"
		"__builtin__.ezp_plot3D=console.ezp_plot3D\n"
		"__builtin__.ezp_save_plot=console.ezp_save_plot\n"
		"import ezp\n"
	);

	// DEBUG: Test functions
	PyRun_SimpleString("def f1(x): return x**2\n");
	PyRun_SimpleString("def g1(x, y): return (sin(x) + sin(y))\n");
	PyRun_SimpleString("def g2(x,y): return (( 1 - (x**2 + y**2) ) * exp( -1/2 * (x**2 + y**2) ))\n");

}
char save_error_type[1024], save_error_info[1024];

bool
EZPMainConsole::py_check_for_unexpected_eof()
{
	PyObject *errobj, *errdata, *errtraceback, *pystring;

	/* get latest python exception info */
	PyErr_Fetch(&errobj, &errdata, &errtraceback);

	pystring = NULL;
	if (errobj != NULL &&
		(pystring = PyObject_Str(errobj)) != NULL &&     /* str(object) */
		(PyString_Check(pystring))
		)
	{
		strcpy(save_error_type, PyString_AsString(pystring));
	}
	else
		strcpy(save_error_type, "<unknown exception type>");
	Py_XDECREF(pystring);

	pystring = NULL;
	if (errdata != NULL &&
		(pystring = PyObject_Str(errdata)) != NULL &&
		(PyString_Check(pystring))
		)
		strcpy(save_error_info, PyString_AsString(pystring));
	else
		strcpy(save_error_info, "<unknown exception data>");
	Py_XDECREF(pystring);

	if (strstr(save_error_type, "exceptions.SyntaxError")!=NULL &&
		strncmp(save_error_info,"('unexpected EOF while parsing',",32)==0)
	{
		return true;
	}
	PyErr_Print ();
	resultString="Error: ";
	resultString.append(save_error_info);
	Py_XDECREF(errobj);
	Py_XDECREF(errdata);         /* caller owns all 3 */
	Py_XDECREF(errtraceback);    /* already NULL'd out */
	return false;
}

//Desctructor
EZPMainConsole::~EZPMainConsole()
{
	for (U32 Idx = 0; Idx < MAX_PLOT_WINDOW_COUNT; Idx++)
	{
		if (RenderWidgetArray[Idx])
		{
			RenderWidgetArray[Idx]->close();
		}

	}
	
	Py_Finalize();
}

//Call the Python interpreter to execute the command
//retrieve back results using the python internal stdout/err redirectory (see above)
QString EZPMainConsole::interpretCommand(const QString &command, int *res)
{
	PyObject* py_result;
	PyObject* dum;
	bool multiline=false;
	*res = 0;
	if (!command.startsWith('#') && (!command.isEmpty() || (command.isEmpty() && lines!=0)))
	{
		this->command.append(command);
		py_result=Py_CompileString(this->command.toLocal8Bit().data(),"<stdin>",Py_single_input);
		if (py_result==0)
		{
			multiline=py_check_for_unexpected_eof();
			if (!multiline) {
				if (command.endsWith(':'))
					multiline = true;
			}

			if (multiline)
			{
				setMultilinePrompt(false);
				this->command.append("\n");
				lines++;
				resultString="";
				QConsole::interpretCommand(command, res);
				return "";
			}
			else
			{
				setNormalPrompt(false);
				*res=-1;
				QString result=resultString;
				resultString="";
				QConsole::interpretCommand(command, res);
				this->command="";
				this->lines=0;
				return result;
			}
		}
		if ( (lines!=0 && command=="") || (this->command!="" && lines==0))
		{
			setNormalPrompt(false);
			this->command="";
			this->lines=0;

			dum = PyEval_EvalCode ((PyCodeObject *)py_result, glb, loc);
			Py_XDECREF (dum);
			Py_XDECREF (py_result);
			if (PyErr_Occurred ())
			{
				*res=-1;
				PyErr_Print ();
			}
			QString result=resultString;
			resultString="";
			if (command!="")
				QConsole::interpretCommand(command, res);
			return result;
		}
		else if (lines!=0 && command!="") //following multiliner line
		{
			this->command.append("\n");
			*res=0;
			QConsole::interpretCommand(command, res);
			return "";
		}
		else
		{
			return "";
		}

	}
	else
		return "";
}

QStringList EZPMainConsole::suggestCommand(const QString &cmd, QString& prefix)
{
	char run[255];
	int n =0;
	QStringList list;
	prefix = "";
	resultString="";
	if (!cmd.isEmpty()) {
		do {
			snprintf(run,255,"print completer.complete(\"%s\",%d)\n",
					 cmd.toLatin1().data(),n);
			PyRun_SimpleString(run);
			resultString=resultString.trimmed(); //strip trialing newline
			if (resultString!="None")
			{
				list.append(resultString);
				resultString="";
			}
			else
			{
				resultString="";
				break;
			}
			n++;
		} while (true);
	}
	list.removeDuplicates();
	return list;
}
