#include "lindi.h"
#include "browser.h"
#include "editor.h"

Lindi::Lindi():
	_shell(*this),
	_browser(new Browser)
{
        std::unique_ptr<UI::Controller> browser(_browser);
        _shell.open_window(std::move(browser));
}

void Lindi::edit_file(std::string path)
{
	// If we already have this file open, bring it forward.
	auto existing = _editors.find(path);
	if (existing != _editors.end()) {
		_shell.make_active(existing->second);
		return;
	}
	// We don't have an editor for this file, so we should create one.
	std::unique_ptr<UI::Controller> ed(new Editor::Controller(path));
	UI::Window *win = _shell.open_window(std::move(ed));
	_editors[path] = win;
}

void Lindi::file_closed(std::string path)
{
	auto iter = _editors.find(path);
	if (iter != _editors.end()) _editors.erase(iter);
}

void Lindi::select_project(std::string path)
{
	_browser->open_project(path);
}

void Lindi::run()
{
	if (_editors.empty()) {
		_browser->show_projects();
	}
	timeout(20);
	do {
		update_panels();
		doupdate();
	} while (!_done && _shell.process(getch()));
}
