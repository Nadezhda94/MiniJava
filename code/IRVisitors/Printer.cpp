#include "Printer.h"

static const char * ArithmeticOpStrings[] = { "+", "-", "*", "/", "&&", "||", "<<", ">>", "!" };
static const char * CJumpOpStrings[] = { "=", "!=", "<", ">", "<=", ">=", "u<", "u<=", "u>", "u>=" };

CIRPrinter::CIRPrinter(ostream& _out): out(_out) {}
CIRPrinter::CIRPrinter(ostream& _out, bool _withoutSEQ) : out(_out), withoutSEQ(_withoutSEQ) {}
void CIRPrinter::Visit(MOVE* node) {
	print_tabs(counter++);
	out << "MOVE" << endl;
	node->dst->accept(this);
	node->src->accept(this);
	--counter;
}

void CIRPrinter::Visit(EXP* node) {
	print_tabs(counter++);
	out << "EXP" << endl;
	node->exp->accept(this);
	--counter;
}

void CIRPrinter::Visit(JUMP* node) {
	print_tabs(counter++);
	out << "JUMP "<< node->target->Name() << endl;
	if (node->exp != 0)
		node->exp->accept(this);
	--counter;
}

void CIRPrinter::Visit(CJUMP* node) {
	print_tabs(counter++);
	out << "CJUMP " << CJumpOpStrings[node->relop] << " " <<
	node->iftrue->Name()<< " " << node->iffalse->Name() << " " << endl;
	node->left->accept(this);
	node->right->accept(this);
	--counter;
}

void CIRPrinter::Visit(SEQ* node) {
	if (!withoutSEQ) {
		print_tabs(counter++);
		out << "SEQ" << endl;
	}
	node->left->accept(this);
	node->right->accept(this);
	if (!withoutSEQ)
		--counter;
}

void CIRPrinter::Visit(LABEL* node) {
	print_tabs(counter++);
	out << "LABEL "<< node->label->Name() << endl;
	--counter;
}

void CIRPrinter::Visit(CONST* node) {
	print_tabs(counter++);
	out << "CONST " << node->value << endl;
	--counter;
}

void CIRPrinter::Visit(NAME* node) {
	print_tabs(counter++);
	out << "NAME " << node->label->Name() << endl;
	--counter;
}

void CIRPrinter::Visit(TEMP* node) {
	print_tabs(counter++);
	out << "TEMP " << node->temp->Name() << endl;
	//node->temp->accept(this);
	--counter;
}

void CIRPrinter::Visit(BINOP* node) {
	print_tabs(counter++);
	out << "BINOP " << ArithmeticOpStrings[node->binop] << endl;
	node->left->accept(this);
	node->right->accept(this);
	--counter;
}

void CIRPrinter::Visit(MEM* node) {
	print_tabs(counter++);
	out << "MEM" << endl;
	node->exp->accept(this);
	--counter;
}

void CIRPrinter::Visit(CALL* node) {
	print_tabs(counter++);
	out << "CALL" << endl;
	node->func->accept(this);
	shared_ptr<ExpList> cur = node->args;
	while(cur) {
		cur->head->accept(this);
		cur = cur->tail;
	}
	--counter;
}

void CIRPrinter::Visit(ESEQ* node) {
	print_tabs(counter++);
	out << "ESEQ" << endl;
	node->stm->accept(this);
	node->exp->accept(this);
	--counter;
}

void CIRPrinter::Visit(IRTree::MoveCall* node) {
	assert(0);
}
void CIRPrinter::Visit(IRTree::ExpCall* node) {
	assert(0);
}

void CIRPrinter::print_tabs(int num) {
	for (int i = 0; i < num; i++)
		out << "| ";
}