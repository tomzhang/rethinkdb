#include "rdb_protocol/op.hpp"
#include "rdb_protocol/err.hpp"

namespace ql {

class error_term_t : public op_term_t {
public:
    error_term_t(env_t *env, const Term2 *term) : op_term_t(env, term, argspec_t(1)) { }
private:
    virtual val_t *eval_impl() {
        throw exc_t(arg(0)->as_datum()->as_str());
    }
    RDB_NAME("error");
};

} //namespace ql
