#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include "blockchain.hpp"

namespace py = pybind11;

PYBIND11_MODULE(backend, m) {

    py::class_<Blockchain>(m, "Blockchain")
        .def(py::init())
        .def("mine_block", &Blockchain::mine)
        .def("check_block_parent", &Blockchain::check_parent)
        .def("get_end_of_chain", &Blockchain::get_end_of_chain)
        .def("set_difficulty", &Blockchain::set_difficulty)
        .def("set_max_iterations", &Blockchain::set_max_iterations)
        .def("get_difficulty", &Blockchain::get_difficulty)
        .def("get_max_iterations", &Blockchain::get_max_iterations)
        .def("get_last_block_index",
             [](const Blockchain &blockchain) {
                 return blockchain.get_end_of_chain().get_index();
             })
        .def("get_last_block_data",
             [](const Blockchain &blockchain) {
                 return blockchain.get_end_of_chain().get_data();
             })
        .def("get_last_block_hash",
             [](const Blockchain &blockchain) {
                 return blockchain.get_end_of_chain().get_hash();
             })
        .def("get_block_hash",
             [](const Blockchain &blockchain, const size_t &block_id) {
                 return blockchain.get_block(block_id).get_hash();
             })
        .def("get_last_block_parent",
             [](const Blockchain &blockchain) {
                 return blockchain.get_end_of_chain().get_parent_hash();
             })
        .def("get_last_block_timestamp",
             [](const Blockchain &blockchain) {
                 return blockchain.get_end_of_chain().get_timestamp();
             })
        .def("get_last_block_nonce",
             [](const Blockchain &blockchain) {
                 return blockchain.get_end_of_chain().get_nonce();
             });

    py::class_<Block>(m, "Block")
        .def(py::init<const size_t, const size_t, const time_t, const std::string, const std::string, const std::string>())
        .def("check_hash", &Block::check_hash);

}
