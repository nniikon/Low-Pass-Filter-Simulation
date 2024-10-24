#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <complex>
#include <cmath>
#include <vector>

namespace py = pybind11;
using namespace std::complex_literals;

double wave(double t, double period, double magnitude, double freq) {
    double square_wave = (t < period) ? 1.0 : 0.0;
    double sine_component = magnitude * std::sin(2 * M_PI * freq * t);
    return square_wave + sine_component;
}

std::pair<py::array_t<std::complex<double>>, py::array_t<double>> sft_cpp(double period, double magnitude, double freq, double t_low, double t_high, double dt) {
    std::vector<double> t_values;
    for (double t = t_low; t < t_high; t += dt) {
        t_values.push_back(t);
    }
    
    std::size_t N = t_values.size();
    double T = t_high - t_low;
    double df = 1.0 / T;
    std::vector<double> freqs(N);
    std::vector<std::complex<double>> fourier_transform(N, 0.0);

    for (std::size_t i = 0; i < N; ++i) {
        freqs[i] = i * df;
        double omega = 2 * M_PI * freqs[i];
        for (std::size_t j = 0; j < N; ++j) {
            double t = t_values[j];
            double signal_value = wave(t, period, magnitude, freq);
            fourier_transform[i] += signal_value * std::exp(-1i * omega * t) * dt;
        }
    }

    return { py::array_t<std::complex<double>>(fourier_transform.size(), fourier_transform.data()),
             py::array_t<double>(freqs.size(), freqs.data()) };
}

py::array_t<double> inverse_sft_cpp(py::array_t<std::complex<double>> fourier_transform, py::array_t<double> freqs, double t_low, double t_high, double dt) {
    std::vector<double> t_values;
    for (double t = t_low; t < t_high; t += dt) {
        t_values.push_back(t);
    }

    std::size_t N = t_values.size();
    double T = t_high - t_low;
    auto fourier_transform_unchecked = fourier_transform.unchecked<1>();
    auto freqs_unchecked = freqs.unchecked<1>();

    std::vector<std::complex<double>> signal_reconstructed(N, 0.0);

    for (std::size_t j = 0; j < N; ++j) {
        double t = t_values[j];
        for (std::size_t i = 0; i < N; ++i) {
            double omega = 2 * M_PI * freqs_unchecked(i);
            signal_reconstructed[j] += fourier_transform_unchecked(i) * std::exp(1i * omega * t) * (1.0 / T);
        }
    }

    std::vector<double> signal_real(N);
    for (std::size_t j = 0; j < N; ++j) {
        signal_real[j] = std::real(signal_reconstructed[j]);
    }

    return py::array_t<double>(signal_real.size(), signal_real.data());
}

PYBIND11_MODULE(fourier_transform, m) {
    m.def("sft", &sft_cpp, "Simple Fourier Transform",
          py::arg("period"), py::arg("magnitude"), py::arg("freq"), 
          py::arg("t_low"), py::arg("t_high"), py::arg("dt"));
    m.def("inverse_sft", &inverse_sft_cpp, "Inverse Simple Fourier Transform");
}
