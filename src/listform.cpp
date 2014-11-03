#include "listform.h"
#include <assert.h>

void ListForm::paint(WINDOW *view)
{
	if (_dirty) {
		_dirty = false;
		refresh();
	}
	int height, width;
	getmaxyx(view, height, width);
	for (int y = 0; y < height; ++y) {
		paint_line(view, y, height, width);
	}
}

bool ListForm::process(WINDOW *view, int ch)
{
        switch (ch) {
                case 258: arrow_down(view); break;
                case 259: arrow_up(view); break;
		case '\r': commit(view); break;
		case 28: escape(view); break;
		default: break;
	}
	return true;
}

bool ListForm::poll(WINDOW *view)
{
	return true;
}

void ListForm::refresh()
{
	// Render our commands and entries down to some text lines.
	_lines.clear();
	Line blank;
	_lines.push_back(blank);
	if (!_commands.empty()) {
		for (auto &field: _commands) {
			_lines.push_back(field->text());
		}
		_lines.push_back(blank);
	}
	if (!_entries.empty()) {
		if (!_entry_label.empty()) {
			_lines.push_back(_entry_label);
		}
		for (auto &field: _entries) {
			_lines.push_back(field.get());
		}
		_lines.push_back(blank);
	}
	// If the cursor has gone out of range, bring it back.
	if (_selpos >= _lines.size()) {
		_selpos = _lines.size();
	}
	// If the cursor is not on a selectable line, look for
	// one further down it might apply to.
	if (!is_selectable(_selpos)) {
		size_t delta = 1;
		while (delta < _lines.size()) {
			if (is_selectable(_selpos + delta)) {
				_selpos += delta;
				break;
			}
			if (is_selectable(_selpos - delta)) {
				_selpos -= delta;
				break;
			}
			++delta;
		}
	}

}

void ListForm::paint_line(WINDOW *view, int y, int height, int width)
{
	size_t line = (size_t)y + _scrollpos;
	int lwid = std::max(0, width - 2);
	if (line < _lines.size()) {
		std::string text = _lines[line].text;
		mvwaddnstr(view, y, 1, text.c_str(), lwid);
	}
	wclrtoeol(view);
	if (line == _selpos) {
		mvwchgat(view, y, 1, lwid, A_REVERSE, 0, NULL);
	}
}

bool ListForm::is_selectable(ssize_t line)
{
	if (line < 0) return false;
	if ((size_t)line >= _lines.size()) return false;
	return _lines[line].field != nullptr;
}

void ListForm::arrow_down(WINDOW *view)
{
	// Look for a selectable line past the current one.
	// If we find one, select it, then repaint.
	// If we don't find one, do nothing.
	for (size_t i = _selpos + 1; i < _lines.size(); ++i) {
		if (!is_selectable(i)) continue;
		_selpos = i;
		paint(view);
		break;
	}
}

void ListForm::arrow_up(WINDOW *view)
{
	// Look for a selectable line before the current one.
	// If we find one, select it, then repaint.
	// If we don't find one, leave the selection alone.
	for (size_t i = _selpos; i > 0; --i) {
		size_t next = i - 1;
		if (!is_selectable(next)) continue;
		_selpos = next;
		paint(view);
		break;
	}
}

void ListForm::commit(WINDOW *view)
{
	// Invoke the selected field.
	assert(_selpos < _lines.size());
	auto &line = _lines[_selpos];
	if (line.field) {
		line.field->invoke();
		_dirty = true;
		paint(view);
	}
}

void ListForm::escape(WINDOW *view)
{
	// If we had a text edit field open, close it.
}

