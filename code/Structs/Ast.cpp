#include "Ast.h"

void CNode::setLocation(int _firstColumn, int _firstLine, int _lastColumn, int _lastLine) {
	location.firstColumn = _firstColumn;
	location.firstLine = _firstLine;
	location.lastColumn = _lastColumn;
	location.lastLine = _lastLine;
}