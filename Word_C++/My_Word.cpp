#include "stdafx.h"
#include "My_Word.h"
#include <io.h>

CMyWord::CMyWord()
{
	m_Wordapp = NULL;
	m_Worddocs = NULL;
	m_Worddoc = NULL;
	m_Wordrange = NULL;
	m_Wordselect = NULL;
}

CMyWord::~CMyWord()
{
	if (m_Worddoc != NULL)
	{
		m_Worddoc.Close(
			COleVariant((short)false),	// SaveChanges.
			COleVariant((short)true),	// OriginalFormat.
			COleVariant((short)false)	// RouteDocument.
			);
		m_Wordapp.Quit(
			COleVariant((short)false),	// SaveChanges.
			COleVariant((short)true),	// OriginalFormat.
			COleVariant((short)false)	// RouteDocument.
			);
		//�ͷ��ڴ�������Դ
		m_Wordrange.ReleaseDispatch();
		m_Wordselect.ReleaseDispatch();
		m_Worddoc.ReleaseDispatch();
		m_Worddocs.ReleaseDispatch();
		m_Wordapp.ReleaseDispatch();
		m_Wordrange = NULL;
		m_Wordselect = NULL;
		m_Worddoc = NULL;
		m_Worddocs = NULL;
		m_Wordapp = NULL;
	}
}

//�½�Word
bool CMyWord::CreateWord(bool isVisible)
{
	if (InitWord(isVisible))
	{
		m_Worddocs = m_Wordapp.get_Documents();
		if (!m_Worddocs.m_lpDispatch)
		{
			AfxMessageBox("Documents����ʧ��!", MB_OK | MB_ICONWARNING);
			return false;
		}
		COleVariant varTrue(short(1), VT_BOOL), vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
		CComVariant Template(_T(""));    //û��ʹ��WORD���ĵ�ģ��
		CComVariant NewTemplate(false), DocumentType(0), Visible;

		//��ӿհ�ҳ
		m_Worddocs.Add(&Template, &NewTemplate, &DocumentType, &Visible);

		// �õ�document����
		m_Worddoc = m_Wordapp.get_ActiveDocument();
		if (!m_Worddoc.m_lpDispatch)
		{
			AfxMessageBox("Document��ȡʧ��!", MB_OK | MB_ICONWARNING);
			return false;
		}
		//�õ�selection����
		m_Wordselect = m_Wordapp.get_Selection();
		if (!m_Wordselect.m_lpDispatch)
		{
			AfxMessageBox("Select��ȡʧ��!", MB_OK | MB_ICONWARNING);
			return false;
		}
		//�õ�Range����
		m_Wordrange = m_Worddoc.Range(vOptional, vOptional);
		if (!m_Wordrange.m_lpDispatch)
		{
			AfxMessageBox("Range��ȡʧ��!", MB_OK | MB_ICONWARNING);
			return false;
		}
		return true;
	}
	return false;
}

//��Word
bool CMyWord::OpenWord(string inWordPath, bool isVisible)
{
	//�ж��ļ��Ƿ����
	if (_access(inWordPath.c_str(), 0) == -1)
	{
		m_Wordapp.Quit(
			COleVariant((short)false),    // SaveChanges.
			COleVariant((short)true),            // OriginalFormat.
			COleVariant((short)false)            // RouteDocument.
			);
		m_Wordapp = NULL;
		MessageBoxA(NULL, "ָ���򿪵��ļ�������!", "��ʾ", MB_ICONEXCLAMATION);
		return FALSE;
	}

	//�ж��ļ��Ƿ���д��Ȩ��
	if (_access(inWordPath.c_str(), 2) == -1)
	{
		m_Wordapp.Quit(
			COleVariant((short)false),    // SaveChanges.
			COleVariant((short)true),            // OriginalFormat.
			COleVariant((short)false)            // RouteDocument.
			);
		m_Wordapp = NULL;
		MessageBoxA(NULL, "ָ���򿪵��ļ�û��д��Ȩ��!", "��ʾ", MB_ICONEXCLAMATION);
		return false;
	}

	//��Word
	if (InitWord(isVisible))
	{
		m_Worddocs = m_Wordapp.get_Documents();
		if (!m_Worddocs.m_lpDispatch)
		{
			AfxMessageBox("Documents����ʧ��!", MB_OK | MB_ICONWARNING);
			return false;
		}
		COleVariant vTrue((short)TRUE),
			vFalse((short)FALSE),
			vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR),
			vZ((short)0);

		//�õ�document����
		m_Worddoc = m_Worddocs.Open(
				(COleVariant)inWordPath.c_str(),        // FileName
				vTrue,            // Confirm Conversion.
				vFalse,           // ReadOnly.
				vFalse,           // AddToRecentFiles.
				vOptional,        // PasswordDocument.
				vOptional,        // PasswordTemplate.
				vOptional,        // Revert.
				vOptional,        // WritePasswordDocument.
				vOptional,        // WritePasswordTemplate.
				vOptional,        // Format. // Last argument for Word 97
				vOptional,        // Encoding // New for Word 2000/2002
				vOptional,        // Visible
				/*����4����word2003��Ҫ�Ĳ��������汾��word2000��*/
				vOptional,    // OpenAndRepair
				vZ,            // DocumentDirection wdDocumentDirection LeftToRight
				vOptional,    // NoEncodingDialog
				vOptional
			);

		if (!m_Worddoc.m_lpDispatch)
		{
			AfxMessageBox("Document��ȡʧ��!", MB_OK | MB_ICONWARNING);
			return FALSE;
		}
		//�õ�selection����
		m_Wordselect = m_Wordapp.get_Selection();
		if (!m_Wordselect.m_lpDispatch)
		{
			AfxMessageBox("Select��ȡʧ��!", MB_OK | MB_ICONWARNING);
			return false;
		}
		//�õ�Range����
		m_Wordrange = m_Worddoc.Range(vOptional, vOptional);
		if (!m_Wordrange.m_lpDispatch)
		{
			AfxMessageBox("Range��ȡʧ��!", MB_OK | MB_ICONWARNING);
			return false;
		}
		this->m_filename = inWordPath;
		return true;
	}
	return false;
}

//����Word
bool CMyWord::SaveWord()
{
	m_Worddoc.Save();
	return TRUE;
}

//���Word-----�����滻
bool CMyWord::SaveAs(string inSavePath)
{
	try
	{
		if (strcmp(inSavePath.c_str(), this->m_filename.c_str()) != 0)
		{
			COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
			m_Worddoc.SaveAs(COleVariant(inSavePath.c_str()), covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, 
				covOptional,  covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional);
			this->m_filename = inSavePath;
			return true;
		}
		else
		{
			MessageBoxA(NULL, "����·����ԭ·����ͬ����ֱ��ʹ��SaveWord()!", "��ʾ", MB_ICONEXCLAMATION);
			return false;
		}
	}
	catch (exception &e)
	{
		MessageBoxA(NULL, e.what(), "����", MB_ICONEXCLAMATION);
		return false;
	}
}

//�ر�Word
bool CMyWord::CloseWord()
{
	m_Worddoc.Close(
		COleVariant((short)false),	// SaveChanges.
		COleVariant((short)true),	// OriginalFormat.
		COleVariant((short)false)	// RouteDocument.
	);
	m_Wordapp.Quit(
		COleVariant((short)false),	// SaveChanges.
		COleVariant((short)true),	// OriginalFormat.
		COleVariant((short)false)	// RouteDocument.
	);

	//�ͷ��ڴ�������Դ
	m_Wordrange.ReleaseDispatch();
	m_Wordselect.ReleaseDispatch();
	m_Worddoc.ReleaseDispatch();
	m_Worddocs.ReleaseDispatch();
	m_Wordapp.ReleaseDispatch();
	m_Wordrange = NULL;
	m_Wordselect = NULL;
	m_Worddoc = NULL;
	m_Worddocs = NULL;
	m_Wordapp = NULL;

	return true;
}

//�ڵ�ǰ��괦��������
void CMyWord::WriteText(string inString)
{
	m_Wordselect.TypeText(inString.c_str());
}

//����д��
void CMyWord::EnterLineWriteText(string inString, int nLineCount)
{
	if (nLineCount <= 0)
	{
		nLineCount = 0;
	}
	for (int i = 0; i < nLineCount; i++)
	{
		m_Wordselect.TypeParagraph();
	}
	WriteText(inString.c_str());
}

//�ĵ���β��д��
void CMyWord::WriteInLastLine(string inString)
{
	m_Wordrange.InsertAfter(inString.c_str());
}

//��괦������
void CMyWord::InsertTable(int inRow, int inColumn, CTable0 &outTable)
{
	VARIANT vtDefault;
	COleVariant vtAuto;
	vtDefault.vt = VT_INT;
	vtDefault.intVal = 1;
	vtAuto.vt = VT_INT;
	vtAuto.intVal = 0;

	CTables0 wordtables = m_Worddoc.get_Tables();
	outTable = wordtables.Add(m_Wordselect.get_Range(), inRow, inColumn, &vtDefault, &vtAuto);
	wordtables.ReleaseDispatch();
}

//�����д������
void CMyWord::WriteCellFromTable(CTable0 inTable, int inRow, int inColumn, string inString)
{
	CCell cell = inTable.Cell(inRow, inColumn);
	cell.Select(); //������ƶ�����Ԫ��
	m_Wordselect.TypeText(inString.c_str());
	cell.ReleaseDispatch();
}

//��괦����ͼƬ����Table�в���ͼƬ��BUG��Ҫ��~���룩
void CMyWord::InsertPicture(string inPicturePath, bool isDelete)
{
	//�ж��ļ��Ƿ����
	if (_access(inPicturePath.c_str(), 0) == -1)
	{
		m_Wordapp.Quit(
			COleVariant((short)false),			// SaveChanges.
			COleVariant((short)true),            // OriginalFormat.
			COleVariant((short)false)            // RouteDocument.
			);
		m_Wordapp = NULL;
		MessageBoxA(NULL, "ָ����Ҫ�����ͼƬ������!", "��ʾ", MB_ICONEXCLAMATION);
		return;
	}
	COleVariant vTrue((short)TRUE),
		vFalse((short)FALSE),
		vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	CnlineShapes cnlineShapes = m_Wordselect.get_InlineShapes();
	CnlineShape cnlineShape = cnlineShapes.AddPicture(inPicturePath.c_str(), vFalse, vTrue, vOptional);
	if (isDelete)
	{
		::DeleteFile(inPicturePath.c_str());
	}
	
}

//�������N��---Enter
void CMyWord::MoveSelect_E(int inN /*= 1*/)
{
	if (inN <= 0)
	{
		inN = 0;
	}
	for (int i = 0; i < inN; i++)
	{
		m_Wordselect.TypeParagraph();
	}
}

//������ǩ�����ù��λ��
void CMyWord::SetSelectToBookMark(string inMarkName)
{
	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	m_Wordselect.GoTo(covOptional, covOptional, covOptional, (COleVariant)inMarkName.c_str());
}

//�ƶ����---��������
void CMyWord::MoveSelect_U(int inLineCount)
{
	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	m_Wordselect.MoveUp(covOptional, COleVariant((short)inLineCount), covOptional);
}
void CMyWord::MoveSelect_D(int inLineCount)
{
	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	m_Wordselect.MoveDown(covOptional, COleVariant((short)inLineCount), covOptional);
}
void CMyWord::MoveSelect_L(int inCharCount)
{
	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	m_Wordselect.MoveLeft(covOptional, COleVariant((short)inCharCount), covOptional);
}
void CMyWord::MoveSelect_R(int inCharCount)
{
	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	m_Wordselect.MoveRight(covOptional, COleVariant((short)inCharCount), covOptional);
}

//��ʼ��Word
bool CMyWord::InitWord(bool isVisible)
{
	if (!m_Wordapp.CreateDispatch(_T("word.application"), NULL))
	{
		AfxMessageBox("����Word������ʧ��!");
		return false;
	}
	else
	{
		m_Wordapp.put_Visible(isVisible);
		return true;
	}
}
