
#ifndef My_Word_H_INCLUDED
#define My_Word_H_INCLUDED

#pragma once

#include "CApplication.h"
#include "CDocument0.h"
#include "CDocuments.h"
#include "CnlineShape.h"
#include "CnlineShapes.h"
#include "CRange.h"
#include "CSelection.h"
#include "CShape.h"
#include "CShapes.h"
#include "CTable0.h"
#include "CTables0.h"
#include "CTextFrame.h"
#include "CCell.h"
#include "CCells.h"
#include "CBookmark0.h"
#include "CBookmarks.h"

#include <string>
#include <vector>

#define DllExport __declspec(dllexport)

using namespace std;

class DllExport CMyWord
{
public:
	CMyWord();
	virtual ~CMyWord();

public:

	//�½�Word
	bool CreateWord(bool isVisible);

	//��Word
	bool OpenWord(string inWordPath, bool isVisible = false);

	//����Word
	bool SaveWord();

	//���Word-----�����滻
	bool SaveAs(string inSavePath);

	//�ر�Word
	bool CloseWord();

	//��ǰ��괦д�ı�
	void WriteText(string inString);

	//��N��д��
	void EnterLineWriteText(string inString, int nLineCount = 1);

	//�ĵ���β��д��
	void WriteInLastLine(string inString);

	//��괦������
	void InsertTable(int inRow, int inColumn, CTable0 &outTable);

	//�����д������
	void WriteCellFromTable(CTable0 inTable, int inRow, int inColumn, string inString);

	//��괦����ͼƬ
	void InsertPicture(string inPicturePath, bool isDelete = true);

	//�������N��
	void MoveSelect_E(int inN = 1);

	//������ǩ�����ù��λ��
	void SetSelectToBookMark(string inMarkName);

	//�ƶ����---��������
	void MoveSelect_U(int inLineCount);
	void MoveSelect_D(int inLineCount);
	void MoveSelect_L(int inCharCount);
	void MoveSelect_R(int inCharCount);

private:
	CApplication	m_Wordapp;
	CDocuments		m_Worddocs;
	CDocument0		m_Worddoc;
	CRange			m_Wordrange;
	CSelection		m_Wordselect;

	string m_filename;

	//��ʼ��Word
	bool InitWord(bool isVisible);

};

#endif