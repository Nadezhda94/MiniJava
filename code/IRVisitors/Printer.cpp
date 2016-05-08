#include "Printer.h"

static const char * ArithmeticOpStrings[] = { "+", "-", "*", "/", "&&", "||", "<<", ">>", "!" };
static const char * CJumpOpStrings[] = { "=", "!=", "<", ">", "<=", ">=", "u<", "u<=", "u>", "u>=" };

CIRPrinter::~CIRPrinter(){
	gv << "}" << endl;
	gv.close();
}
CIRPrinter::CIRPrinter(ostream& _out): out(_out) {
	gv.open("IRTree.gv");
	gv << "digraph IRTree {" << endl;
}
CIRPrinter::CIRPrinter(ostream& _out, bool _withoutSEQ) : out(_out), withoutSEQ(_withoutSEQ) {
		gv.open("IRTree.gv");
		gv << "digraph IRTree {" << endl;
}
void CIRPrinter::Visit(MOVE* node) {
	print_tabs(counter++);
	int newCount = count++;
	out << "MOVE" << endl;
	gv << "\"" << newCount << "MOVE\"->";
	node->dst->accept(this);
	gv << "\"" << newCount << "MOVE\"->";
	node->src->accept(this);
	--counter;
}

void CIRPrinter::Visit(EXP* node) {
	print_tabs(counter++);
	int newCount = count++;
	out << "EXP" << endl;
	gv << "\"" << newCount << "EXP\"->";
	node->exp->accept(this);
	--counter;
}

void CIRPrinter::Visit(JUMP* node) {
	print_tabs(counter++);
	int newCount = count++;
	out << "JUMP "<< node->target->Name() << endl;
	gv << "\"" << newCount << "JUMP\"->";
	if (node->exp != 0)
		node->exp->accept(this);
	--counter;
}

void CIRPrinter::Visit(CJUMP* node) {
	print_tabs(counter++);
	int newCount = count++;
	out << "CJUMP " << CJumpOpStrings[node->relop] << " " <<
	node->iftrue->Name()<< " " << node->iffalse->Name() << " " << endl;
	gv << "\"" << newCount << "CJUMP\"->";
	node->left->accept(this);
	gv << "\"" << newCount << "CJUMP\"->";
	node->right->accept(this);
	--counter;
}

void CIRPrinter::Visit(SEQ* node) {
	int newCount = count++;
	if (!withoutSEQ) {
		print_tabs(counter++);
		gv << "\"" << newCount << "SEQ\"->";
		out << "SEQ" << endl;
	}
	node->left->accept(this);
	if (!withoutSEQ) gv << "\"" << newCount << "SEQ\"->";
	node->right->accept(this);
	if (!withoutSEQ)
		--counter;
}

void CIRPrinter::Visit(LABEL* node) {
	print_tabs(counter++);
	int newCount = count++;
	gv << "\"" << newCount << "LABEL\"" << endl;
	out << "LABEL "<< node->label->Name() << endl;
	--counter;
}

void CIRPrinter::Visit(CONST* node) {
	print_tabs(counter++);
		int newCount = count++;
	gv << "\"" << newCount << "CONST\"" << endl;
	out << "CONST " << node->value << endl;
	--counter;
}

void CIRPrinter::Visit(NAME* node) {
	print_tabs(counter++);
	int newCount = count++;
	gv << "\"" << newCount << "NAME\"" << endl;
	out << "NAME " << node->label->Name() << endl;
	--counter;
}

void CIRPrinter::Visit(TEMP* node) {
	print_tabs(counter++);
	int newCount = count++;
	gv << "\"" << newCount << "TEMP\"" << endl;
	out << "TEMP " << node->temp->Name() << endl;
	//node->temp->accept(this);
	--counter;
}

void CIRPrinter::Visit(BINOP* node) {
	print_tabs(counter++);
	int newCount = count++;
	out << "BINOP " << ArithmeticOpStrings[node->binop] << endl;
	gv << "\"" << newCount << "BINOP\"->";
	node->left->accept(this);
	gv << "\"" << newCount << "BINOP\"->";
	node->right->accept(this);
	--counter;
}

void CIRPrinter::Visit(MEM* node) {
	print_tabs(counter++);
	int newCount = count++;
	out << "MEM" << endl;
	gv << "\"" << newCount << "MEM\"->";
	node->exp->accept(this);
	--counter;
}

void CIRPrinter::Visit(CALL* node) {
	print_tabs(counter++);
	int newCount = count++;
	out << "CALL" << endl;
	gv << "\"" << newCount << "CALL\"->";
	node->func->accept(this);
	shared_ptr<ExpList> cur = node->args;
	while(cur) {
		gv << "\"" << newCount << "CALL\"->";
		cur->head->accept(this);
		cur = cur->tail;
	}
	--counter;
}

void CIRPrinter::Visit(ESEQ* node) {
	print_tabs(counter++);
	int newCount = count++;
	out << "ESEQ" << endl;
	gv << "\"" << newCount << "ESEQ\"->";
	node->stm->accept(this);
	gv << "\"" << newCount << "ESEQ\"->";
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
