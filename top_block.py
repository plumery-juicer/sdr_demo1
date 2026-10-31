#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: pluto1
# GNU Radio version: 3.10.12.0

from PyQt5 import Qt
from gnuradio import qtgui
from gnuradio import analog
import math
from gnuradio import blocks
import pmt
from gnuradio import digital
from gnuradio import filter
from gnuradio.filter import firdes
from gnuradio import gr
from gnuradio.fft import window
import sys
import signal
from PyQt5 import Qt
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
from gnuradio import eng_notation
from gnuradio import iio
from gnuradio import zeromq
import satellites.hier
import sip
import threading



class top_block(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "pluto1", catch_exceptions=True)
        Qt.QWidget.__init__(self)
        self.setWindowTitle("pluto1")
        qtgui.util.check_set_qss()
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except BaseException as exc:
            print(f"Qt GUI: Could not set Icon: {str(exc)}", file=sys.stderr)
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("gnuradio/flowgraphs", "top_block")

        try:
            geometry = self.settings.value("geometry")
            if geometry:
                self.restoreGeometry(geometry)
        except BaseException as exc:
            print(f"Qt GUI: Could not restore geometry: {str(exc)}", file=sys.stderr)
        self.flowgraph_started = threading.Event()

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 1000000
        self.rx_int_bw = rx_int_bw = 960000
        self.sps = sps = 47
        self.rx_interfere_channel_freq = rx_interfere_channel_freq = 432800000
        self.rx_int_fir_taps = rx_int_fir_taps = firdes.low_pass(1, samp_rate, rx_int_bw / 2, 20000)
        self.rx_info_channel_freq = rx_info_channel_freq = 433200000
        self.rx_center_freq = rx_center_freq = 433100000
        self.frequen_b = frequen_b = 1000000
        self.bt = bt = 0.35

        ##################################################
        # Blocks
        ##################################################

        self.zeromq_pub_msg_sink_0_1 = zeromq.pub_msg_sink('tcp://127.0.0.1:5556', 100, True)
        self.satellites_sync_to_pdu_packed_0 = satellites.hier.sync_to_pdu_packed(
            packlen=19,
            sync="0010111101101111010011000111010010111001000101000100100100101110",
            threshold=4,
        )
        self.rx_interfere_gaussian_filter = filter.fir_filter_fff(1, firdes.gaussian(1.0, sps, bt, int(3*sps) + 1))
        self.rx_interfere_gaussian_filter.declare_sample_delay(0)
        self.rx_interfere_channel_filter = filter.fir_filter_ccc(1, rx_int_fir_taps)
        self.rx_interfere_channel_filter.declare_sample_delay(0)
        self.qtgui_time_sink_x_0_2 = qtgui.time_sink_f(
            1024, #size
            samp_rate, #samp_rate
            "", #name
            1, #number of inputs
            None # parent
        )
        self.qtgui_time_sink_x_0_2.set_update_time(0.10)
        self.qtgui_time_sink_x_0_2.set_y_axis(-1, 1)

        self.qtgui_time_sink_x_0_2.set_y_label('Amplitude', "")

        self.qtgui_time_sink_x_0_2.enable_tags(True)
        self.qtgui_time_sink_x_0_2.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_0_2.enable_autoscale(False)
        self.qtgui_time_sink_x_0_2.enable_grid(False)
        self.qtgui_time_sink_x_0_2.enable_axis_labels(True)
        self.qtgui_time_sink_x_0_2.enable_control_panel(False)
        self.qtgui_time_sink_x_0_2.enable_stem_plot(False)


        labels = ['Signal 1', 'Signal 2', 'Signal 3', 'Signal 4', 'Signal 5',
            'Signal 6', 'Signal 7', 'Signal 8', 'Signal 9', 'Signal 10']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ['blue', 'red', 'green', 'black', 'cyan',
            'magenta', 'yellow', 'dark red', 'dark green', 'dark blue']
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]
        styles = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1]


        for i in range(1):
            if len(labels[i]) == 0:
                self.qtgui_time_sink_x_0_2.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_time_sink_x_0_2.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0_2.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0_2.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0_2.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0_2.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0_2.set_line_alpha(i, alphas[i])

        self._qtgui_time_sink_x_0_2_win = sip.wrapinstance(self.qtgui_time_sink_x_0_2.qwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_time_sink_x_0_2_win)
        self.qtgui_time_sink_x_0 = qtgui.time_sink_c(
            1024, #size
            samp_rate, #samp_rate
            "", #name
            1, #number of inputs
            None # parent
        )
        self.qtgui_time_sink_x_0.set_update_time(0.10)
        self.qtgui_time_sink_x_0.set_y_axis(-1, 1)

        self.qtgui_time_sink_x_0.set_y_label('Amplitude', "")

        self.qtgui_time_sink_x_0.enable_tags(True)
        self.qtgui_time_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_0.enable_autoscale(False)
        self.qtgui_time_sink_x_0.enable_grid(False)
        self.qtgui_time_sink_x_0.enable_axis_labels(True)
        self.qtgui_time_sink_x_0.enable_control_panel(False)
        self.qtgui_time_sink_x_0.enable_stem_plot(False)


        labels = ['Signal 1', 'Signal 2', 'Signal 3', 'Signal 4', 'Signal 5',
            'Signal 6', 'Signal 7', 'Signal 8', 'Signal 9', 'Signal 10']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ['blue', 'red', 'green', 'black', 'cyan',
            'magenta', 'yellow', 'dark red', 'dark green', 'dark blue']
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]
        styles = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1]


        for i in range(2):
            if len(labels[i]) == 0:
                if (i % 2 == 0):
                    self.qtgui_time_sink_x_0.set_line_label(i, "Re{{Data {0}}}".format(i/2))
                else:
                    self.qtgui_time_sink_x_0.set_line_label(i, "Im{{Data {0}}}".format(i/2))
            else:
                self.qtgui_time_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0.set_line_alpha(i, alphas[i])

        self._qtgui_time_sink_x_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0.qwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_time_sink_x_0_win)
        self.iio_pluto_source_0 = iio.fmcomms2_source_fc32('192.168.3.1' if '192.168.3.1' else iio.get_pluto_uri(), [True, True], 32768)
        self.iio_pluto_source_0.set_len_tag_key('packet_len')
        self.iio_pluto_source_0.set_frequency(433200000)
        self.iio_pluto_source_0.set_samplerate(samp_rate)
        self.iio_pluto_source_0.set_gain_mode(0, 'manual')
        self.iio_pluto_source_0.set_gain(0, 40)
        self.iio_pluto_source_0.set_quadrature(True)
        self.iio_pluto_source_0.set_rfdc(True)
        self.iio_pluto_source_0.set_bbdc(True)
        self.iio_pluto_source_0.set_filter_params('Auto', '', 0, 0)
        self.iio_pluto_sink_0_0 = iio.fmcomms2_sink_fc32('192.168.3.1' if '192.168.3.1' else iio.get_pluto_uri(), [True, True], 32768, False)
        self.iio_pluto_sink_0_0.set_len_tag_key('')
        self.iio_pluto_sink_0_0.set_bandwidth(1000000)
        self.iio_pluto_sink_0_0.set_frequency(432800000)
        self.iio_pluto_sink_0_0.set_samplerate(samp_rate)
        self.iio_pluto_sink_0_0.set_attenuation(0, 10)
        self.iio_pluto_sink_0_0.set_filter_params('Auto', '', 0, 0)
        self.iio_pluto_sink_0 = iio.fmcomms2_sink_fc32('192.168.2.1' if '192.168.2.1' else iio.get_pluto_uri(), [True, True], 32768, False)
        self.iio_pluto_sink_0.set_len_tag_key('')
        self.iio_pluto_sink_0.set_bandwidth(1000000)
        self.iio_pluto_sink_0.set_frequency(433200000)
        self.iio_pluto_sink_0.set_samplerate(samp_rate)
        self.iio_pluto_sink_0.set_attenuation(0, -40)
        self.iio_pluto_sink_0.set_filter_params('Auto', '', 0, 0)
        self.digital_symbol_sync_xx_0 = digital.symbol_sync_ff(
            digital.TED_ZERO_CROSSING,
            47,
            0.005,
            1.0,
            1.0,
            1.5,
            1,
            digital.constellation_bpsk().base(),
            digital.IR_MMSE_8TAP,
            128,
            [])
        self.digital_gfsk_mod_0_0_0 = digital.gfsk_mod(
            samples_per_symbol=47,
            sensitivity=0.6517,
            bt=0.35,
            verbose=False,
            log=False,
            do_unpack=True)
        self.digital_gfsk_mod_0_0 = digital.gfsk_mod(
            samples_per_symbol=47,
            sensitivity=1.5628,
            bt=0.35,
            verbose=False,
            log=False,
            do_unpack=True)
        self.digital_binary_slicer_fb_0 = digital.binary_slicer_fb()
        self.blocks_vector_source_x_0 = blocks.vector_source_b((0,1,0,127,0,0,1), True, 1, [])
        self.blocks_file_source_0 = blocks.file_source(gr.sizeof_char*1, '/home/plumery/sdr_project/zmq_send1/build/tx_data.bin', True, 0, 0)
        self.blocks_file_source_0.set_begin_tag(pmt.PMT_NIL)
        self.analog_rail_ff_0 = analog.rail_ff((-1.2), 1.2)
        self.analog_quadrature_demod_cf_0 = analog.quadrature_demod_cf(1.5628)
        self.analog_agc_xx_0 = analog.agc_cc((1e-5), 0.5, 1.0, 1000)


        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.satellites_sync_to_pdu_packed_0, 'out'), (self.zeromq_pub_msg_sink_0_1, 'in'))
        self.connect((self.analog_agc_xx_0, 0), (self.analog_quadrature_demod_cf_0, 0))
        self.connect((self.analog_quadrature_demod_cf_0, 0), (self.analog_rail_ff_0, 0))
        self.connect((self.analog_rail_ff_0, 0), (self.rx_interfere_gaussian_filter, 0))
        self.connect((self.blocks_file_source_0, 0), (self.digital_gfsk_mod_0_0, 0))
        self.connect((self.blocks_vector_source_x_0, 0), (self.digital_gfsk_mod_0_0_0, 0))
        self.connect((self.digital_binary_slicer_fb_0, 0), (self.satellites_sync_to_pdu_packed_0, 0))
        self.connect((self.digital_gfsk_mod_0_0, 0), (self.iio_pluto_sink_0, 0))
        self.connect((self.digital_gfsk_mod_0_0_0, 0), (self.iio_pluto_sink_0_0, 0))
        self.connect((self.digital_symbol_sync_xx_0, 0), (self.digital_binary_slicer_fb_0, 0))
        self.connect((self.digital_symbol_sync_xx_0, 1), (self.qtgui_time_sink_x_0_2, 0))
        self.connect((self.iio_pluto_source_0, 0), (self.qtgui_time_sink_x_0, 0))
        self.connect((self.iio_pluto_source_0, 0), (self.rx_interfere_channel_filter, 0))
        self.connect((self.rx_interfere_channel_filter, 0), (self.analog_agc_xx_0, 0))
        self.connect((self.rx_interfere_gaussian_filter, 0), (self.digital_symbol_sync_xx_0, 0))


    def closeEvent(self, event):
        self.settings = Qt.QSettings("gnuradio/flowgraphs", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        self.stop()
        self.wait()

        event.accept()

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.set_rx_int_fir_taps(firdes.low_pass(1, self.samp_rate, self.rx_int_bw / 2, 20000))
        self.iio_pluto_sink_0.set_samplerate(self.samp_rate)
        self.iio_pluto_sink_0_0.set_samplerate(self.samp_rate)
        self.iio_pluto_source_0.set_samplerate(self.samp_rate)
        self.qtgui_time_sink_x_0.set_samp_rate(self.samp_rate)
        self.qtgui_time_sink_x_0_2.set_samp_rate(self.samp_rate)

    def get_rx_int_bw(self):
        return self.rx_int_bw

    def set_rx_int_bw(self, rx_int_bw):
        self.rx_int_bw = rx_int_bw
        self.set_rx_int_fir_taps(firdes.low_pass(1, self.samp_rate, self.rx_int_bw / 2, 20000))

    def get_sps(self):
        return self.sps

    def set_sps(self, sps):
        self.sps = sps
        self.rx_interfere_gaussian_filter.set_taps(firdes.gaussian(1.0, self.sps, self.bt, int(3*self.sps) + 1))

    def get_rx_interfere_channel_freq(self):
        return self.rx_interfere_channel_freq

    def set_rx_interfere_channel_freq(self, rx_interfere_channel_freq):
        self.rx_interfere_channel_freq = rx_interfere_channel_freq

    def get_rx_int_fir_taps(self):
        return self.rx_int_fir_taps

    def set_rx_int_fir_taps(self, rx_int_fir_taps):
        self.rx_int_fir_taps = rx_int_fir_taps
        self.rx_interfere_channel_filter.set_taps(self.rx_int_fir_taps)

    def get_rx_info_channel_freq(self):
        return self.rx_info_channel_freq

    def set_rx_info_channel_freq(self, rx_info_channel_freq):
        self.rx_info_channel_freq = rx_info_channel_freq

    def get_rx_center_freq(self):
        return self.rx_center_freq

    def set_rx_center_freq(self, rx_center_freq):
        self.rx_center_freq = rx_center_freq

    def get_frequen_b(self):
        return self.frequen_b

    def set_frequen_b(self, frequen_b):
        self.frequen_b = frequen_b

    def get_bt(self):
        return self.bt

    def set_bt(self, bt):
        self.bt = bt
        self.rx_interfere_gaussian_filter.set_taps(firdes.gaussian(1.0, self.sps, self.bt, int(3*self.sps) + 1))




def main(top_block_cls=top_block, options=None):

    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()

    tb.start()
    tb.flowgraph_started.set()

    tb.show()

    def sig_handler(sig=None, frame=None):
        tb.stop()
        tb.wait()

        Qt.QApplication.quit()

    signal.signal(signal.SIGINT, sig_handler)
    signal.signal(signal.SIGTERM, sig_handler)

    timer = Qt.QTimer()
    timer.start(500)
    timer.timeout.connect(lambda: None)

    qapp.exec_()

if __name__ == '__main__':
    main()
