# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# def options(opt):
#     pass

# def configure(conf):
#     conf.check_nonfatal(header_name='stdint.h', define_name='HAVE_STDINT_H')

def build(bld):
    module = bld.create_ns3_module('raa-eval', ['core','network','internet','wifi','applications'])
    module.source = [
        'model/configure-topology.cc',
        'model/traffic-parameters.cc',
        'model/hidden-station-topology.cc',
        ]

    module_test = bld.create_ns3_module_test_library('raa-eval')
    module_test.source = [
        'test/raa-eval-test-suite.cc',
        ]

    headers = bld(features='ns3header')
    headers.module = 'raa-eval'
    headers.source = [
        'model/configure-topology.h',
        'model/traffic-parameters.h',
        'model/hidden-station-topology.h',
        ]

    if bld.env.ENABLE_EXAMPLES:
        bld.recurse('examples')

    # bld.ns3_python_bindings()
