/**
 * Copyright (c) 2020-2021 Paul-Louis Ageneau
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "track.hpp"
#include "globals.hpp"

#include "impl/track.hpp"

namespace rtc {

Track::Track(impl_ptr<impl::Track> impl)
    : CheshireCat<impl::Track>(impl), Channel(std::dynamic_pointer_cast<impl::Channel>(impl)) {}

string Track::mid() const { return impl()->mid(); }

Description::Direction Track::direction() const { return impl()->direction(); }

Description::Media Track::description() const { return impl()->description(); }

void Track::setDescription(Description::Media description) {
	impl()->setDescription(std::move(description));
}

void Track::close() { impl()->close(); }

bool Track::send(message_variant data) { return impl()->outgoing(make_message(std::move(data))); }

bool Track::send(const byte *data, size_t size) { return send(binary(data, data + size)); }

bool Track::isOpen(void) const { return impl()->isOpen(); }

bool Track::isClosed(void) const { return impl()->isClosed(); }

size_t Track::maxMessageSize() const {
	return DEFAULT_IPV4_MTU - 12 - 8 - 20; // SRTP/UDP/IPv4
}

void Track::setRtcpHandler(std::shared_ptr<MediaHandler> handler) {
	impl()->setRtcpHandler(std::move(handler));
}

bool Track::requestKeyframe() {
	if (auto handler = impl()->getRtcpHandler())
		return handler->requestKeyframe();

	return false;
}

std::shared_ptr<MediaHandler> Track::getRtcpHandler() { return impl()->getRtcpHandler(); }

} // namespace rtc
