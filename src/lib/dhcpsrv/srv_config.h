// Copyright (C) 2014 Internet Systems Consortium, Inc. ("ISC")
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES WITH
// REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR ANY SPECIAL, DIRECT,
// INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
// OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

#ifndef DHCPSRV_CONFIG_H
#define DHCPSRV_CONFIG_H

#include <dhcpsrv/cfg_iface.h>
#include <dhcpsrv/logging_info.h>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <stdint.h>

namespace isc {
namespace dhcp {

class CfgMgr;


/// @brief Specifies current DHCP configuration
///
/// @todo Migrate all other configuration parameters from cfgmgr.h here
class SrvConfig {
public:
    /// @name Constants for selection of parameters returned by @c getConfigSummary
    ///
    //@{
    /// Nothing selected
    static const uint32_t CFGSEL_NONE    = 0x00000000;
    /// Number of IPv4 subnets
    static const uint32_t CFGSEL_SUBNET4 = 0x00000001;
    /// Number of IPv6 subnets
    static const uint32_t CFGSEL_SUBNET6 = 0x00000002;
    /// Number of enabled ifaces
    static const uint32_t CFGSEL_IFACE4  = 0x00000004;
    /// Number of v6 ifaces
    static const uint32_t CFGSEL_IFACE6  = 0x00000008;
    /// DDNS enabled/disabled
    static const uint32_t CFGSEL_DDNS    = 0x00000010;
    /// Number of all subnets
    static const uint32_t CFGSEL_SUBNET  = 0x00000003;
    /// IPv4 related config
    static const uint32_t CFGSEL_ALL4    = 0x00000015;
    /// IPv6 related config
    static const uint32_t CFGSEL_ALL6    = 0x0000001A;
    /// Whole config
    static const uint32_t CFGSEL_ALL     = 0xFFFFFFFF;
    //@}

    /// @brief Default constructor.
    ///
    /// This constructor sets configuration sequence number to 0.
    SrvConfig();

    /// @brief Constructor.
    ///
    /// Sets arbitrary configuration sequence number.
    SrvConfig(uint32_t sequence);

    /// @brief Returns summary of the configuration in the textual format.
    ///
    /// This method returns the brief text describing the current configuration.
    /// It may be used for logging purposes, e.g. when the new configuration is
    /// committed to notify a user about the changes in configuration.
    ///
    /// @todo Currently this method uses @c CfgMgr accessors to get the
    /// configuration parameters. Once these parameters are migrated from the
    /// @c CfgMgr this method will have to be modified accordingly.
    ///
    /// @todo Implement reporting a summary of interfaces being used for
    /// receiving and sending DHCP messages. This will be implemented with
    /// ticket #3512.
    ///
    /// @param selection Is a bitfield which describes the parts of the
    /// configuration to be returned.
    ///
    /// @return Summary of the configuration in the textual format.
    std::string getConfigSummary(const uint32_t selection) const;

    /// @brief Returns configuration sequence number.
    uint32_t getSequence() const {
        return (sequence_);
    }

    /// @brief Compares configuration sequence with other sequence.
    ///
    /// This method compares sequence numbers of two configurations for
    /// equality. The sequence numbers are meant to be unique, so if
    /// they are equal it means that they point to the same configuration.
    ///
    /// @param other Configuration which sequence number should be
    /// compared with the sequence number of this configuration.
    ///
    /// @return true if sequence numbers are equal.
    bool sequenceEquals(const SrvConfig& other);

    /// @brief Returns logging specific configuration.
    const LoggingInfoStorage& getLoggingInfo() const {
        return (logging_info_);
    }

    /// @brief Sets logging specific configuration.
    ///
    /// @param logging_info New logging configuration.
    void addLoggingInfo(const LoggingInfo& logging_info) {
        logging_info_.push_back(logging_info);
    }

    /// @brief Returns object which represents selection of interfaces.
    ///
    /// This function returns a reference to the object which represents the
    /// set of interfaces being used to receive DHCP traffic.
    ///
    /// @return Object representing selection of interfaces.
    const CfgIface& getCfgIface() const {
        return (cfg_iface_);
    }

    /// @brief Sets the object representing selection of interfaces.
    ///
    /// @param cfg_iface Object representing selection of interfaces.
    void setCfgIface(const CfgIface& cfg_iface) {
        cfg_iface_ = cfg_iface;
    }

    /// @brief Copies the currnet configuration to a new configuration.
    ///
    /// This method copies the parameters stored in the configuration to
    /// an object passed as parameter. The configuration sequence is not
    /// copied.
    ///
    /// @param [out] new_config An object to which the configuration will
    /// be copied.
    void copy(SrvConfig& new_config) const;

    /// @brief Apply logging configuration to log4cplus.
    void applyLoggingCfg() const;

    /// @name Methods and operators used to compare configurations.
    ///
    //@{
    ///
    /// @brief Compares two objects for equality.
    ///
    /// It ignores the configuration sequence number when checking for
    /// equality of objects.
    ///
    /// @param other An object to be compared with this object.
    ///
    /// @return true if two objects are equal, false otherwise.
    bool equals(const SrvConfig& other) const;

    /// @brief Compares two objects for inequality.
    ///
    /// It ignores the configuration sequence number when checking for
    /// inequality of objects.
    ///
    /// @param other An object to be compared with this object.
    ///
    /// @return true if two objects are not equal, false otherwise.
    bool nequals(const SrvConfig& other) const {
        return (!equals(other));
    }

    /// @brief Equality operator.
    ///
    /// It ignores the configuration sequence number when checking for
    /// equality of objects.
    ///
    /// @param other An object to be compared with this object.
    ///
    /// @return true if two objects are equal, false otherwise.
    bool operator==(const SrvConfig& other) const {
        return (equals(other));
    }

    /// @param other An object to be compared with this object.
    ///
    /// It ignores the configuration sequence number when checking for
    /// inequality of objects.
    ///
    /// @param other An object to be compared with this object.
    ///
    /// @return true if two objects are not equal, false otherwise.
    bool operator!=(const SrvConfig& other) const {
        return (nequals(other));
    }

    //@}

private:

    /// @brief Sequence number identifying the configuration.
    uint32_t sequence_;

    /// @brief Logging specific information.
    LoggingInfoStorage logging_info_;

    /// @brief Interface configuration.
    ///
    /// Used to select interfaces on which the DHCP server will listen to
    /// queries.
    CfgIface cfg_iface_;

};

/// @name Pointers to the @c SrvConfig object.
///
//@{
/// @brief Non-const pointer to the @c SrvConfig.
typedef boost::shared_ptr<SrvConfig> SrvConfigPtr;

/// @brief Const pointer to the @c SrvConfig.
typedef boost::shared_ptr<const SrvConfig> ConstSrvConfigPtr;
//@}

} // namespace isc::dhcp
} // namespace isc

#endif // DHCPSRV_CONFIG_H
