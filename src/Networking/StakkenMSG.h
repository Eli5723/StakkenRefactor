#pragma once

enum class StakkenMSG {
    ServerAccept,
    ServerDeny,
    ServerPing,
    
    AuthGuest,
    AuthUser,
    AuthAccept,
    AuthReject,
};
