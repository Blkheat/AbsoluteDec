#ifndef __IEXPLORE_H
#define __IEXPLORE_H

extern "C" long DisplayHTMLPage(HWND hwnd, wchar_t *addr, TCHAR *post);
extern "C" long DisplayHTMLPageW(HWND hwnd, wchar_t *addr, TCHAR *post);
extern "C" LRESULT CALLBACK WebWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern "C" void UnEmbedBrowserObject(HWND hwnd);
extern "C" long EmbedBrowserObject(HWND hwnd);
#endif