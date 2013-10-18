# -*- coding: cp1252 -*-

import os
import sys
import traceback
import subprocess
import argparse
import re
import configparser

def remove_slash(path):
    return path.rstrip('/').rstrip('\\')

def quote(str):
    return '"' + str + '" '
    
def type_file(file_name):
    if not os.access(file_name, os.F_OK):
        return
    file = open(file_name, 'r')
    for line in file:
        sys.stdout.write(line)
    file.close
                
def generate_opt_file(nparams, args):
    file_name = args.module + 'Opt' + str(nparams) + '.txt'
    file = open(file_name, 'w')

    file.write('-s "#arg_count#" -r "' + str(nparams) + '"\n')
    
    file.write('-s "#template_args#" -r "class F')
    for n in range(1,nparams+1):
        file.write(', class P' + str(n))
    file.write('"\n')
        
    file.write('-s "#function_args#" -r "F f, Context context')
    for n in range(1,nparams+1):
        file.write(', P' + str(n) + ' p' + str(n))
    file.write('"\n')
        
    file.write('-s "#init_list#" -r "f(f)')
    for n in range(1,nparams+1):
        file.write(', p' + str(n) + '(p' + str(n) + ')')
    file.write('"\n')
        
    file.write('-s "#func_call_args#" -r "')
    for n in range(1,nparams+1):
        file.write(', p' + str(n))
    file.write('"\n')
        
    file.write('-s "#member_def#" -r "F f;')
    for n in range(1,nparams+1):
        file.write(' P' + str(n) + ' p' + str(n) + ';')
    file.write('"\n')
        
    file.write('-s "#template_call_args#" -r "F')
    for n in range(1,nparams+1):
        file.write(', P' + str(n))
    file.write('"\n')
    
    file.close        

def generate_batch_file(args):
    file_name = 'Make' + args.module + '.bat'
    file = open(file_name, 'w')
    
    template_file_name = args.module + '.txt'
    output_file_name = '..\\' + args.module + '.hpp'
    file.write('type Header.txt >' + output_file_name + '\n')
    for n in range(int(args.maxparam) + 1):
        opt_file_name = args.module + 'Opt' + str(n) + '.txt'
        file.write('..\\..\\..\\..\\wintools\\ssr\\ssr --options ' + opt_file_name + ' -f ' + template_file_name + ' -o body.txt\n')
        file.write('type body.txt >>' + output_file_name + '\n')    
    file.write('type Footer.txt >>' + output_file_name + '\n')
            
    file.close
        

#
# main
#
def main():
    arg_parser = argparse.ArgumentParser(description='Make Code Generation Script')
    arg_parser.add_argument(
        '--module',
        dest='module',
        metavar='name',
        help='The script to generate.',
        required=True)
    arg_parser.add_argument(
        '--maxparam',
        dest='maxparam',
        metavar='number',
        help='The maximum number of parameters the template will support.',
        required=True)
        
    args = arg_parser.parse_args()
    
    file_name = 'Make' + args.module + '.bat'
    
    out_file = open(file_name, 'w')
    
    for nparams in range(int(args.maxparam) + 1):
        generate_opt_file(nparams, args)
        
    generate_batch_file(args)
        
    #for nparams in range(args.maxparam)
    #    generate(out_file, args)
        
    out_file.close
    
    return 0
                           
if __name__ == '__main__':
    try:
        sys.exit(main())
    except Exception as error:
        traceback.print_exc()
        sys.exit(1)
