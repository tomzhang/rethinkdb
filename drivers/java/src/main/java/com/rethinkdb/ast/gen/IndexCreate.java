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

public class IndexCreate extends RqlQuery {

    public IndexCreate(RqlAst prev, Arguments args, OptionalArguments optargs) {
        super(prev, TermType.INDEX_CREATE, args, optargs);
    }
    /* Query level terms */
}
