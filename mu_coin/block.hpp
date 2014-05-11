#pragma once
#include <boost/multiprecision/cpp_int.hpp>
#include <mu_coin/address.hpp>
#include <mu_coin/delta.hpp>
#include <cryptopp/eccrypto.h>
#include <cryptopp/osrng.h>
#include <cryptopp/oids.h>
#include <unordered_map>

namespace mu_coin {
    using EC = CryptoPP::ECDSA <CryptoPP::ECP, CryptoPP::SHA256>;
    CryptoPP::RandomNumberGenerator & pool ();
    CryptoPP::OID & curve ();
    union uint256_union
    {
        uint256_union () = default;
        uint256_union (boost::multiprecision::uint256_t const &);
        uint256_union (EC::PublicKey const &);
        std::array <uint8_t, 32> bytes;
        std::array <uint64_t, 4> qwords;
        void clear ();
        boost::multiprecision::uint256_t number ();
    };
    union uint512_union
    {
        uint512_union () = default;
        uint512_union (boost::multiprecision::uint512_t const &);
        std::array <uint8_t, 64> bytes;
        std::array <uint64_t, 8> qwords;
        void clear ();
        boost::multiprecision::uint512_t number ();
    };
    class block
    {
    public:
        uint512_union signature;
        virtual void sign (EC::PrivateKey const &) = 0;
        virtual bool validate (EC::PublicKey const &) = 0;
        virtual boost::multiprecision::uint256_t fee () const = 0;
        virtual boost::multiprecision::uint256_t hash () const = 0;
    };
    class entry
    {
    public:
        entry () = default;
        entry (boost::multiprecision::uint256_t const &, boost::multiprecision::uint256_t const &);
        boost::multiprecision::uint256_t previous;
        boost::multiprecision::uint256_t coins;
    };
    class transaction_block : public mu_coin::block
    {
    public:
        boost::multiprecision::uint256_t fee () const override;
        boost::multiprecision::uint256_t hash () const override;
        void sign (EC::PrivateKey const &) override;
        bool validate (EC::PublicKey const &) override;
        std::unordered_map <mu_coin::address, entry> entries;
    };
    class delegate_block : public mu_coin::block
    {
    public:
        delegate_block (boost::multiprecision::uint256_t const &);
        boost::multiprecision::uint256_t fee () const override;
        boost::multiprecision::uint256_t hash () const override;
        void sign (EC::PrivateKey const &) override;
        boost::multiprecision::uint256_t previous;
        mu_coin::address address;
    };
}