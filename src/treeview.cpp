#include "treeview.h"
#include "repolist.h"
#include <dirent.h>

TreeView::TreeView(Browser &host, std::string path):
	_host(host),
	_dirpath(path)
{
	enumerate(path, 0);
}

void TreeView::render(Fields &fields)
{
	fields.entry("Switch Repository", [this](){switchrepo();});
	fields.blank();
	for (auto &entry: _entries) {
		fields.entry(entry.text, [](){});
	}
}

void TreeView::switchrepo()
{
	std::unique_ptr<Controller> sub(new RepoList(_host));
	_host.delegate(std::move(sub));
}

void TreeView::enumerate(std::string path, unsigned indent)
{
	DIR *pdir = opendir(path.c_str());
	if (!pdir) return;
	while (dirent *entry = readdir(pdir)) {
		if (entry->d_name[0] == '.') continue;
		std::string name(entry->d_name);
		std::string subpath(path + "/" + name);
		switch (entry->d_type) {
		case DT_DIR: subdir(name, subpath, indent); break;
		case DT_REG: subfile(name, subpath, indent); break;
		}
	}
	closedir(pdir);
}

void TreeView::subdir(std::string name, std::string subpath, unsigned indent)
{
	std::string text = tab(indent) + name + "/";
	_entries.emplace_back(entry(text, subpath));
	enumerate(subpath, indent + 1);
}

void TreeView::subfile(std::string name, std::string subpath, unsigned indent)
{
	std::string text = tab(indent) + name;
	_entries.emplace_back(entry(text, subpath));
}

std::string TreeView::tab(unsigned indent)
{
	std::string out;
	out.resize(indent * 4, ' ');
	return out;
}

