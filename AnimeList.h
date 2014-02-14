#ifndef _AnimeList_AnimeList_h
#define _AnimeList_AnimeList_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <AnimeList/AnimeList.lay>
#define IMAGECLASS AImage
#define IMAGEFILE <AnimeList/AnimeList.iml>
#include <Draw/iml.h>
#include <CtrlCore/lay.h>

enum Items {
	Complete, 
	Named,
	NamedRus,
	Episodes,
	Views,
	Release,
	SeriesRelease,
	ID,
	Series = 0,
	DateRelease};

int Items_name_len = 8;
char *Items_name[] = {
	"Complete",
	"Name",
	"Russian",
	"Episodes",
	"Views",
	"Release",
	"SeriesRelease",
	"ID"};


struct Cell {
	String complete;
	String name;
	String nameRus;
	int episodes;
	int views;
	Date Release;
	String releaseDate;
};

struct CellSeries {
	int number;
	Date release;
};



class AnimeList : public WithAnimeListLayout<TopWindow> {
	WithInfoLayout<ParentCtrl> infoTab;
	WithAddLayout<ParentCtrl> addTab;
	WithModifyLayout<TopWindow> dlg;
	WithCalendarLayout<ParentCtrl> clndr;
	WithParametersLayout<ParentCtrl> parameters;
	WithTestInfoLayout<ParentCtrl> test;
	
	FileSel fs;
	String filename;
	bool size;
	Cell cell;
	CellSeries series;
	Array<Cell> mainArray;
	Array<CellSeries> seriesArray;
	
	DropList drop;
	EditString name, nameRus, copy;
	EditInt ep1, ep2;
	DropDate date;

	struct ColumnRed : Display {
		virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const 
		{
			ink = White();
			paper = Red();
			w.DrawRect(r, paper);
			w.DrawText(r.left, r.top, q.ToString(),StdFont(),ink);
		}
		virtual void PaintBackground(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
		{
			paper = Red();
			w.DrawRect(r, paper);
		}
	};
	
	struct ColumnGreen : Display {
		virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const 
		{
			ink = White();
			paper = Green();
			w.DrawRect(r, paper);
			w.DrawText(r.left, r.top, q.ToString(),StdFont(),ink);
		}
		virtual void PaintBackground(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
		{
			paper = Green();
			w.DrawRect(r, paper);
		}
	};
	
	struct ColumnBlue : Display {
		virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const 
		{
			ink = White();
			paper = Blue();
			w.DrawRect(r, paper);
			w.DrawText(r.left, r.top, q.ToString(),StdFont(),ink);
		}
		virtual void PaintBackground(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
		{
			paper = Blue();
			w.DrawRect(r, paper);
		}
	};
	
	struct ColumnDefault : Display {
		virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const 
		{
			ink = Black();
			paper = White();
			w.DrawRect(r, paper);
			w.DrawText(r.left, r.top, q.ToString(),StdFont(),ink);
		}
		virtual void PaintBackground(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
		{
			paper = White();
			w.DrawRect(r, paper);
		}
	};

	void FileMenu(Bar& bar);
	void MainMenu(Bar& bar);
	void RightMenu(Bar& bar);
	
	void Add();
	void AnimeSeries();
	void Change();
	void ChangeSeries();
	void CloseFile();
	void CreateArray();
	void DoClose();
	void EditValue();
	void FullSizeWindow();
	void MinusViews();
	void Modify();
	void ModifyAnimeSeries();
	void ModifyDate();
	void ModifySeries();
	void Open();
	void OpenFile();
	void OpenLastFile();
	void OpenModify();
	void PlusViews();
	void PrintFile();
	void Remove();
	void ResetParameters();
	void RowColor(int row);
	void Save();
	void SaveAs();
	void SeriesEdit();
	void SetupYears();
	void Sorts();
	void SortsComplete();
	void SortsLooks();
	void SortsSeasons();
	void SortsYears();
	void Quit();
public:
	typedef AnimeList CLASSNAME;
	AnimeList();
	void Test();
};

#endif
