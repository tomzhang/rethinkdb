// Autogenerated by convert_protofile.py on 2015-05-04.
// Do not edit this file directly.
// The template for this file is located at:
// ../../../../../../../../templates/AstSubclass.java
package com.rethinkdb.ast.gen;

import com.rethinkdb.Cursor;
import com.rethinkdb.ast.helper.Arguments;
import com.rethinkdb.ast.helper.OptionalArguments;
import com.rethinkdb.ast.RqlAst;
import com.rethinkdb.proto.TermType;
import java.util.*;

public class Error extends RqlQuery {

    public Error(RqlAst prev, Arguments args, OptionalArguments optargs) {
        super(prev, TermType.ERROR, args, optargs);
    }
    /* Query level terms */
}
