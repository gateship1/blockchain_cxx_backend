#!/usr/bin/env python3

from flask import Flask
from flask import request, jsonify, make_response
from flask_cors import CORS

import sys
sys.path.append(r'../out/build/blockchain-server/lib/')
import backend

app = Flask(__name__)
CORS(app)

@app.route('/', methods=['GET', 'POST'])
def index():
    
    if request.method == 'GET':
        response_body = {
            "blockid": blockchain.get_last_block_index(),
            "parent": blockchain.get_last_block_parent(),
            "timestamp": blockchain.get_last_block_timestamp(),
            "dataval": blockchain.get_last_block_data(),
            "hash": blockchain.get_last_block_hash(),
            "nonce": blockchain.get_last_block_nonce(),
            "miningdifficulty": blockchain.get_difficulty(),
            "miningmaxiter": blockchain.get_max_iterations(),
        }
        res = make_response(jsonify(response_body), 200)
        return res
    
    elif request.method == 'POST' and request.is_json:
        
        req = request.get_json()
        cmd = req.get("cmd")
        if cmd == 'mine':
            mining_difficulty = req.get("difficulty")
            datastr = req.get("data")
            max_iter = req.get("maxiterations")
            parent_hash = req.get("parent")
            
            if (blockchain.check_block_parent(parent_hash)):
                blockchain.set_difficulty(mining_difficulty)
                blockchain.set_max_iterations(max_iter)
                if (blockchain.mine_block(datastr)):
                    response_body = {
                        "miningdifficulty": blockchain.get_difficulty(),
                        "maximumiterations": blockchain.get_max_iterations(),
                        "blockid": blockchain.get_last_block_index(),
                        "parent": blockchain.get_last_block_parent(),
                        "timestamp": blockchain.get_last_block_timestamp(),
                        "data": blockchain.get_last_block_data(),
                        "hash": blockchain.get_last_block_hash(),
                        "nonce": blockchain.get_last_block_nonce(),
                    }
                else:
                    response_body = {
                        "error": "max iterations exceeded"
                    }
            else:
                response_body = {
                    "error": "parent does not match last block on the chain"
                }
        elif cmd == "check_block":
            datastr = req.get("data")
            parent_hash = req.get("parent")
            hashstr = "junk"
            bid = req.get("blockid")
            nonce = req.get("nonce")
            timestamp = req.get("timestamp")
            
            block = backend.Block(nonce, bid, timestamp, parent_hash, datastr, hashstr)
            
            if block.check_hash() == blockchain.get_block_hash(bid):
                response_body = {
                     "matches": "true"
                }
            else:
                response_body = {
                     "matches": "false"
                }
        elif cmd == "check_difficulty":
            if req.get("difficulty") >= blockchain.get_difficulty():
                response_body = {
                     "acceptable": "true"
                }
            else:
                response_body = {
                     "acceptable": "false"
                }
        elif cmd == "get_difficulty":
            response_body = {
                "miningdifficulty": blockchain.get_difficulty()
            }
        elif cmd == "get_max_iter":
            response_body = {
                "miningmaxiter": blockchain.get_max_iterations()
            }
        else:
            response_body = {
                "error": "unrecognized command: command missing or incorrect"
            }           
        res = make_response(jsonify(response_body), 200)
        return res
    return make_response(jsonify({"message": "Request body must be JSON"}), 400)

if __name__ == '__main__':

    blockchain = backend.Blockchain()     
    app.run(debug=False, host='0.0.0.0')
